>repop_prog 100~
mpadd_qmark self roulette_dealer -1 0 0 0 0 1
mpadd_qmark self roulette_random_timer -1 0 0 0 0 1
mpforcer here def_rembch $i
~
>rand_prog 100~
if is_qmark($i) == roulette_dealer 100001
	if is_qmark($i) == roulette_random_timer 100002
		mpforcer here rcall roulette_timeline $i
		mpmod_qmark self roulette_random_timer v0 = 0
	else
		if rand(80)
			if rand(90)
				mpforcer here rcall roulette_timeline $i
			else
				mpmod_qmark self roulette_random_timer v0 + 1
			endif
		else
			if rand(50)
				mpforcer here rcall roulette_timeline $i
			else
				mpmod_qmark self roulette_random_timer v0 + 1
			endif
		endif
	endif
endif
~
>talk_prog topic~
tell $n ·Q¤F¸Ñ¥»½ü½Lªºª±ªk¶Ü¡H(roulette-rule)
~
>talk_prog p roulette-rule~
tell $n ¥u­n¹ïµÛ·Q­nªº¦ì¸m¤Uª`§Y¥i¥[¤J¹CÀ¸¡A³Ì²×½ü½Lªº¼Æ¦r»P¤Uª`¼Æ¦r¬Û¦P«h¥i¥H±o¨ì¤£¦P½ß²vªº³ø¹S¡C
tell $n ¤Uª`ªº¤èªk¬° bet '¦ì¸m ¼Æ¦r®æ¦¡'¡A¨C¦¸¤Uª`¬° 1 ³æ¦ì¹S½X¡A¦P¼Ë¦ì¸m¥u¥i¥H¤U¤@¦¸¡A¦P³õ¹CÀ¸¤º³Ì¦h¤Uª` 10 ¦¸¡C
tell $n ¦b¹CÀ¸¤º¯àÀH®É¨Ï¥Î check ¨Ó¬Ý¥x®àªº¼Ë»ª¤Î¥Ø«e¦Û¤v¤Uª`ªº±¡ªp¡C
tell $n ¤Uª`¦ì¸m¦³¡G³æ¼Æ¦r(single)¡B¼Æ¦r¶¡(split)¡B¦C(street)¡B¨¤(corner)¡B±Æ(line)¡B¦æ(column)¡B¥´(dozen)¡B
tell $n ¡@¡@¡@¡@¡@¡@©_¼Æ(odd)¡B°¸¼Æ(even)¡B¢°¡ã¢°¢·(1to18)¡B¢°¢¸¡ã¢²¢µ(19to36)¡B¶Â(black)¡B¬õ(red)¡C
tell $n ¨CºØ¤Uª`¦ì¸m³£¦³¤£¦Pªº¼Æ¦r®æ¦¡¤U­±±N³v¤@¸ÑÄÀ¡C(roulette-rule2)
~
>talk_prog p roulette-rule2~
tell $n ³æ¼Æ¦r(single)¡Gbet 'single [1;37m¸¹½X[1;33m'
tell $n ¦¹¤èªk¯àª½±µ¤Uª`¦b¥ô¤@¼Æ¦r¤W¡A¦Ó¸¹½X¬° 0 ¨ì 36¡C
tell $n ¨Ò¡G¤Uª`¸¹½X 0 «h¿é¤J bet 'single 0'¡A¤Uª`¸¹½X 3 «h¿é¤J bet 'single 3'
tell $n ------------------------------------------------------------------------------
tell $n ¼Æ¦r¶¡(split)¡Gbet 'split [1;37m¸¹½X1[1;33m',[1;37m¸¹½X2[1;33m'
tell $n ¦b¬Û¾Fªº¨â¸¹½X¶¡ªº½u¤W¤Uª`¡A¯à¤@¦¸¿ïª` 2 ­Ó¼Æ¦r¡A¦ý¬O¤£¥]§t¸¹½X 0¡C
tell $n ¸¹½X1 »P ¸¹½X2 ¬°¥x®à¤W¨â­Ó¬Û¾Fªº¸¹½X¡A¸¹½X¬O§_¬Û¾F¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¨Ò¡G¤Uª` 1 »P 2 ¤§¶¡«h¿é¤J bet 'split 1,2'¡A¤Uª` 4 »P 7 ¤§¶¡«h¿é¤J bet 'split 4,7'
tell $n µù¡G¨â¼Æ¦r¥H , ²Å¸¹°Ï¹j¡A¶¶§Ç¥i¥HÅÜ§ó¡C
tell $n ------------------------------------------------------------------------------
tell $n ¦C(street)¡Gbet 'street [1;37m³Ì¤p¸¹½X[1;33m'to[1;37m³Ì¤j¸¹½X[1;33m'
tell $n ¦b²Ä¤@¦æ(column 1)©³½u¤W©ó³æ¼Æ½u¤W¤Uª`¡A¯à¿ïª`¤@¦C 3 ­Ó¸¹½X¡A¦ý¤£¥]§t¸¹½X 0¡C
tell $n ¤@¦C¤¤³Ì¤pªº¼Æ¦r¬°³Ì¤p¸¹½X¡A¦Ó³Ì¤jªº¼Æ¦r«h¬O³Ì¤j¸¹½X¡A¨C­Ó¦C¥]§tªº¸¹½X¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¨Ò¡G¤Uª` 1, 2, 3 ªº¦C«h¿é¤J bet 'street 1to3'¡A¤Uª` 25, 26, 27 ªº¦C«h¿é¤J bet 'street 25to27'
tell $n ------------------------------------------------------------------------------
tell $n ¨¤(corner)¡Gbet 'corner down[1;37m¼Æ¦r[1;33m' ©Î bet 'corner upper[1;37m¼Æ¦r[1;33m'
tell $n ¥|­Ó¸¹½Xªº¥æ¤eÂIºÙ¬°¨¤¡A¦b¨¤¤Uª`¯à¤@¦¸¿ïª` 4 ­Ó¼Æ¦r¡A¦ý¤£¥]§t¸¹½X 0¡C
tell $n ¥x®à¤W¦@¦³ 22 ­Ó¨¤¡A¥Ñ²Ä¤G¦æ(column 2)ªº¦ì¸m°Ï¤À¬°¤U(down)»P¤W(upper)¡A¼Æ¦r¥Ñ¥ª¨ì¥k¼W¥[¡A¤À§O¬° 1 ¦Ü 11¡C
tell $n ¨C­Ó¨¤¥]§tªº¸¹½X¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¨Ò¡G¤Uª` 1, 2, 4, 5 ªº¨¤«h¿é¤J bet 'corner down1'¡A¤Uª` 29, 30, 32, 33 ªº¨¤«h¿é¤J bet 'corner upper10'
tell $n ------------------------------------------------------------------------------
tell $n ±Æ(line)¡Gbet 'line [1;37m³Ì¤p¸¹½X[1;33m'to[1;37m³Ì¤j¸¹½X[1;33m'
tell $n ¦b²Ä¤@¦æ(column 1)©³½u¤W©ó¨â¼Æ¶¡½u¤W¤Uª`¡A¯à¿ïª`¤@±Æ 6 ­Ó¸¹½X¡A¦ý¤£¥]§t¸¹½X 0¡C
tell $n ¤@±Æ¤¤³Ì¤pªº¼Æ¦r¬°³Ì¤p¸¹½X¡A¦Ó³Ì¤jªº¼Æ¦r«h¬O³Ì¤j¸¹½X¡A¨C­Ó±Æ¥]§tªº¸¹½X¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¨Ò¡G¤Uª` 1, 2, 3, 4, 5, 6 ªº±Æ«h¿é¤J bet 'line 1to6'
tell $n ¡@¡@¤Uª` 22, 23, 24, 25, 26 ªº±Æ«h¿é¤J bet 'street 22to27'
tell $n ------------------------------------------------------------------------------
tell $n ¦æ(column)¡Gbet 'column [1;37m¦æ¼Æ[1;33m'
tell $n ¹ï¯S©w¦æ¼Æ¤Uª`¯à¤@¦¸¿ïª`¤@¦æ¤¤ªº 12 ­Ó¼Æ¦r¡A¦ý¤£¥]§t¸¹½X 0¡C
tell $n ¨C­Ó¦æ¥]§tªº¸¹½X¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¨Ò¡G¤Uª`²Ä¤@¦æ«h¿é¤J bet 'column 1'¡A¤Uª`²Ä¤G¦æ«h¿é¤J bet 'column 2'¡A¤Uª`²Ä¤T¦æ«h¿é¤J bet 'column 3'
tell $n ------------------------------------------------------------------------------
tell $n ¥´(dozen)¡Gbet 'dozen [1;37m²Ä´X¥´[1;33m'
tell $n ¹ï¯S©w¥´¼Æ¤Uª`¯à¤@¦¸¿ïª`¤@¥´¤¤ªº 12 ­Ó¼Æ¦r¡A¦ý¤£¥]§t¸¹½X 0¡C
tell $n ¨C­Ó¥´¥]§tªº¸¹½X¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¨Ò¡G¤Uª`²Ä¤@¥´«h¿é¤J bet 'dozen 1'¡A¤Uª`²Ä¤G¥´«h¿é¤J bet 'dozen 2'¡A¤Uª`²Ä¤T¥´«h¿é¤J bet 'dozen 3'
tell $n ------------------------------------------------------------------------------
tell $n °¸¼Æ(even)¡Gbet even¡A¿ïª`¥x®à¤W¤£¥]§t 0 ªº©Ò¦³°¸¼Æ¸¹½X¡C
tell $n ©_¼Æ(odd)¡Gbet odd¡A¿ïª`¥x®à¤W¤£¥]§t 0 ªº©Ò¦³©_¼Æ¸¹½X¡C
tell $n ¢°¡ã¢°¢·(1to18)¡Gbet 1to18¡A¿ïª`¸¹½X 1 ¦Ü 18¡C
tell $n ¢°¢¸¡ã¢²¢µ(19to36)¡Gbet 19to36¡A¿ïª`¸¹½X 19 ¦Ü 36¡C
tell $n ¬õ(red)¡Gbet red¡A¿ïª`¬õ¦â¸¹½X¡A¸¹½XªºÃC¦â¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ¶Â(black)¡Gbet black¡A¿ïª`¶Â¦â¸¹½X¡A¸¹½XªºÃC¦â¥i¥Ñ check «ü¥O¬d¬Ý¡C
tell $n ------------------------------------------------------------------------------
tell $n ¦pªGÁÙ·Q¤F¸Ñ©ã¤¤¦U¦ì¸mªº½ß²v¥i¥HÄ~Äò°Ý§Ú¡C(roulette-rule3)
~
>talk_prog p roulette-rule3~
tell $n ¨C¦¸¤Uª`¬° 1 ³æ¦ì¡A¦P³õ¹CÀ¸¤º¨C¦ì¶¢®a³Ì¦h¤Uª` 10 ¦¸¡C
tell $n ---------------------------------------------
tell $n ³æ¼Æ¦r(single)                    1 ½ß 35
tell $n ¼Æ¦r¶¡(split)                     1 ½ß 17
tell $n ¦C(street)                        1 ½ß 11
tell $n ¨¤(corner)                        1 ½ß 8
tell $n ±Æ(line)                          1 ½ß 5
tell $n ¦æ(column)                        1 ½ß 2
tell $n ¥´(dozen)                         1 ½ß 2
tell $n °¸¼Æ(even)                        1 ½ß 1
tell $n ©_¼Æ(odd)                         1 ½ß 1
tell $n ¢°¡ã¢°¢·(1to18)                   1 ½ß 1
tell $n ¢°¢¸¡ã¢²¢µ(19to36)                1 ½ß 1
tell $n ¬õ(red)                           1 ½ß 1
tell $n ¶Â(black)                         1 ½ß 1
tell $n ---------------------------------------------
tell $n ¥t¥~¥»®à»O¯à§I´«¤â¤W¤j¤p¤£¦PªºÄw½X(chips)¡C
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