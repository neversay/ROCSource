>greet_prog 100~
	if is_qmark($n) == forest_enemy 100000
		if is_qmark($n) != forest_quest
			mpechoat $n $I ¤f¤¤¤£ª¾¹D³ä³ä°áµÛ¤°»òªF¦è....¡I
			mpechoaround $n $I ©¿µM´£°_ªZ¾¹©¹ $n ½Ä¤F¹L¥h....¡I
			mpechoat $n $I ©¿µM´£°_ªZ¾¹©¹§A½Ä¤F¹L¨Ó....¡I
			mpat 5 invoke
			hunt $n
		else
			mpechoat $n $I »¡¡G¬°¤°»òÀÝ±þ§Ú­Ì±ÚùØªº¤H....¡I
			mpechoat $n ´d¼«ªº $I ®³°_ªZ¾¹¦V§A½Ä¤F¹L¨Ó....¡I
			mpechoaround $n $I ©¿µM´£°_ªZ¾¹©¹ $n ½Ä¤F¹L¥h....¡I
		endif
		break
	endif
	if is_qmark($i) == forest_fiesta 300000
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if is_qmark($n) !& forest_quest 300256
			mpechoat $n §A¨£¨ì¤F $I ¡A§A¤ß·Q¡G¸Ó«ç»ò¶}©l(start)¸ò¥L»¡©O¡H
		endif
		break
	endif
	if ispc($n)
		if class($n) == 13
or class($n) == 14
or class($n) == 15
			if is_qmark($n) != forest_barbarian_exam
				if is_qmark($n) < forest_quest 100009
					mpechoat $n $I »¡¡G§A¨ìÅ]¤O¤§¶ðªº¤G¼Ó¶i¦æ³¥ÆZ¤H¸Õ·Ò¤F¶Ü¡H
				endif
			endif
		endif
		break
	endif
~
>talk_prog topic~
	if is_qmark($i) == forest_fiesta 300000
		if is_qmark($n) != forest_quest
			if is_qmark($i) > forest_fiesta 200024
				if is_qmark($i) < forest_fiesta 200048
					mpechoat $n $I ¥¿¦bÀRÀRªº¬èÃ«¡A§¹¥þ¤£·Q²z§A¡C
				endif
			else
				mpechoat $n $I »¡¡GAST Q ADHGDY GDYRER VHX DDY DSYTVESTV
				mpechoat $n §A§¹¥þÅ¥¤£À´¥L¨ì©³¦b»¡¨Ç¤°»ò¡C
				mpechoat $n ¤£¹L§A¥i¥H©úÅã·PÄ±¥X¨Ó³o¸ÌÂ×¦~²½ªº¼ö¾x®ðª^....¡C
			endif
		else
			if is_qmark($i) < forest_fiesta 200013
or is_qmark($i) > forest_fiesta 200047
				if rand(50)
					mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
					mpechoat $n $I »¡¡G§A¦n¡A $n ¡A­n¤£­n¤@°_¸õ­Ó»R....¡C
					mpechoat $n ±µµÛ $I ©ÔµÛ§Aªº¤â¡A´N¸õ°_»R¨Ó¤F....¡C
					mpechoaround $n $I ©ÔµÛ $n ªº¤â¸õ°_»R¨Ó¤F....¡C
				else
					mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
					mpechoat $n $I »¡¡G³o¤£¬O $n §a¡IÅwªï°Ñ¥[§Ú­ÌªºÂ×¦~²½¡C
					mpechoat $n $I »¡¡G³o¬O§Ú­Ì¬¼Ây°Êª«ªº¦×¡A¨Ó¡A§A¦Y¦Y¬Ý....¡C
					mpoload 4253
					give leg $n
					mpforce $n eat leg
					gig
					mpechoat $n $I »¡¡G¦p¦ó¡H·PÄ±¤£¿ù§a¡I
				endif
				break
			endif
			if is_qmark($i) > forest_fiesta 200012
				if is_qmark($i) < forest_fiesta 200025
					if rand(50)
						mpechoat $n $I »¡¡G«z¡I¦Yªº¦n¹¡¡A§A¦Û¤vª±§a¡I
						gig
					else
						mpechoat $n $I ¾Kªº¤£¬Ù¤H¨Æ¡A§AÁÙ¬O¤£­nºÞ¥L¦n¤F¡C
					endif
				endif
				break
			endif
			if is_qmark($i) > forest_fiesta 200024
				if is_qmark($i) < forest_fiesta 200048
					mpechoat $n $I ÀRÀRªº¦b¥D«ù¬èÃ«ªº»ö¦¡¡A§¹¥þ¤£²z§A¡C
				endif
			endif
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100010
		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		mpechoat $n $I »¡¡G±z¦n¡A§Ú­ÌÆZ¯î´ËªLªº­^¶¯¡I
		break
	endif
	if is_qmark($n) == forest_quest 100001
		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		mpechoat $n $I »¡¡G±z¦n¡A§Ú¬O³o­Ó¤g¤H³¡¸¨ªº­º»â¡C
		mpechoat $n $I »¡¡G§Ú·Q¦³Ãö³o¸Ìªº¤@¨Ç¨Æ±¡¡A§A¤w¸g¬Û·íªºÁA¸Ñ¤F¡C
		mpechoat $n $I »¡¡G§Ú³oÃä¦³¤@¨Ç¨Æ±¡(quest)»Ý­n§A¿Ë¨­¥h§¹¦¨¡A
		mpechoat $n $I »¡¡G¦pªG§A§â¥L­Ìµ¹§¹¦¨¤F¡A´N¥Nªí§A¦³«e©¹¤õ¤s²`³B®ø·ÀÁÉ¤Ú¬¥¥q¡C
		mpechoat $n $I »¡¡G§Ú·|´£¨Ñ§Ú©Ò¦³ªº¨ó§U¡AÀ°§U§A®ø·À¤õÀs¡C
		mpmod_qmark $n forest_quest v0 + 1
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) == forest_heart 100001
			 if is_inv_obj($n) == 4248
			 	mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			 	mpechoat $n $I »¡¡G§Ö§â[1;36m¥¨«¬¤õôÆë`ªº¤ßÅ¦(heart of huge fire newt)[0m®³µ¹§Ú§a¡I
			 endif
			 break
		endif
		if is_qmark($n) == forest_scale 100001
			 if is_inv_obj($n) == 4244
			 	mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			 	mpechoat $n $I »¡¡G§Ö§â[1;36m³D¦ZªºÅì¤ù(scale of snake queen)[0m®³µ¹§Ú§a¡I
			 endif
			 break
		endif
		if is_qmark($n) == forest_eyes 100001
			 if is_inv_obj($n) == 4245
			 	mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			 	mpechoat $n $I »¡¡G§Ö§â[1;36m·à¤ýªº²´·ú(eyes of lion king)[0m®³µ¹§Ú§a¡I
			 endif
			 break
		endif
		if is_qmark($n) == forest_skin 100001
			 if is_inv_obj($n) == 4246
			 	mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			 	mpechoat $n $I »¡¡G§Ö§â[1;36m¬}¥Þ¥¨¤H¤ýªº¥Ö½§(skin of troll king)[0m®³µ¹§Ú§a¡I
			 endif
			 break
		endif
		if is_qmark($n) < forest_burgeon 100003
			 if is_inv_obj($n) == 4247
			 	mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			 	mpechoat $n $I »¡¡G§Ö§â[1;36m¯«¤ì¤§³»ªº¹àªÞ(burgeon of old tree)[0m®³µ¹§Ú§a¡I
			 endif
			 break
		endif
		break
	endif
	if is_qmark($n) != forest_quest
		mpechoat $n $I »¡¡GGEWR AG AEWTABTS SDY ESVEUUs SEVY QW¡I
		mpechoat $n §A§¹¥þÅ¥¤£À´¥L¨ì©³¦b»¡¨Ç¤°»ò¡C
	endif
~
>talk_prog identify~
	if is_qmark($n) & forest_quest 300512
		if is_qmark($n) !& forest_quest 301024
			mpecho $I ªºÁy¤WÅS¥X¤Q¤ÀªY³ßªº¯º®e....¡C
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§Ú­Ìªº®¦¤H¡A¨S·Q¨ìÁÙ±o³Â·Ð§A°µ³oºØÂû¤ò»[¥Öªº¨Æ¡C
			mpechoat $n $I »¡¡G§Ú¬Û«H§A°V½mªº¦¨ªG¡A§Ú¦P·N¥L­Ìªº±B¨Æ¤F¡C
			mpechoat $n ¦ã½¬®R(Alleina) »P ÀN§J¡]Hawk) ¨â­Ó¤Hºòºòªº©ê¦b¤@°_¡C
			mpechoat $n $I »¡¡G[1;36m¦n¤F¡A§A­Ì¨â­Ó¥ýÂ÷¶}§a¡I§Ú¦³¸Ü(words)­n¸ò $n »¡¡C[0m
			mpecho ÀN§J »P ¦ã½¬®R ¤â²oµÛ¤â¨ì¥~­±¥h³}¤F....¡C
			mpmod_qmark $n forest_quest v2 + 1024
			mptransfer hawk
			mppurge hawk
			mptransfer alleina
			mppurge alleina
		endif
	endif
~
>talk_prog words~
	if is_qmark($n) & forest_quest 301024
		mpechoat $n $I ¨£ ÀN§J »P ¦ã½¬®R ³£Â÷¶}¤F¤§«á¡A©¿µM¦V§A¸÷¤F¤U¨Ó¡C
		mpechoaround $n $I ¨£ ÀN§J »P ¦ã½¬®R ³£Â÷¶}¤F¤§«á¡A©¿µM¦V $n ¸÷¤F¤U¨Ó¡C
		mpechoat $n §A³s¦£±N $I µ¹§ß¤F°_¨Ó....¡C
		mpechoround $n $n ³s¦£±N $I µ¹§ß¤F°_¨Ó¡C
		mpechoat $n $I »¡¡GÁÂÁÂ¡A³s§Ú¤k¨àªº²×¥Í¤j¨Æ³£­n³Â·Ð±z¡A¯u¬O....¡C
		mpechoat $n $I »¡¡G§Ú³£¤£ª¾¹D«ç»ò·PÁÂ±z¤F¡C
		mpechoat $n [1;32m§A¤ß·Q¡G¬O§_À³¸Ó°Ý°Ý¥L­Ì¦ó®ÉÁ|¿ì±BÂ§(marriage)©O¡H[0m
	endif
~
>talk_prog marriage~
	 if is_qmark($n) & forest_quest 301024
	 	if is_qmark($n) !& forest_quest 302048
	 		hop
	 		mpechoat $n $I »¡¡G±z¯à°÷¨Ó°Ñ¥[·íµM¬O³Ì¦n¤£¹Lªº¤F¡C
	 		mpechoat $n $I »¡¡G¦pªG¤p¤k»PÀN§Jªº±BÂ§Á|¦æ¡A§Ú¤@©w¥ß¨è¥h³qª¾±z¡C
	 		mpechoat $n $I »¡¡G¤£¹L±zÀ°¤F§Ú­Ì¤g¤H³¡¸¨³o»ò¦hªº¦£¡A§Ú®É¦bµL¥H¦^³ø¡C
	 		mpechoat $n $I »¡¡G§Ú³o§âÀH¨­ªº[1;36mÅ]ªk¿û¤M(Magicla Steel Knife)[0m´N°eµ¹±z§a¡I[0m
	 		mpoload 4215 36
	 		mpat 5 c enc knife
	 		mpmod_qmark $n forest_quest v2 + 2048
	 		give knife $n
	 		wave $n
	 	endif
	 endif
~
>talk_prog train~
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				bow $n
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n $I »¡¡G¹ê¦b¤Ó·PÁÂ±z¤F¡A§Æ±æ±z¯à±aµÛ¤p¤l¥X¥h¨£¨£¥@­±¡C
				mpechoat $n $I »¡¡G ÀN§J ®Ú¥»¤£ª¾¹D¥~­±¥@¬Éªº¤¿ÀI¡C
				mpforce hawk blush
				mpechoat $n $I »¡¡GÀHµÛ ÀN§J ¾Ô°«¸gÅçªº´£¤É¡A¥L·|ºCºC¶i¨B¡C
				mpechoat $n $I »¡¡G[1;36m·í ÀN§J ¯u¥¿ªº¥Ñ¾Ô°«¤¤ÁA¸Ñ³¥ÆZ¤H¾Ô°«§Þ¥©ªººëÅè®É¡A[0m
				mpechoat $n $I »¡¡G[1;36m§Ú´N§â¤k¨à¶ùµ¹¥L¡C·í ÀN§J ·Ç³Æ(ready)¦n¡A´N¥i¥H¥Xµo¡C[0m
				mpforce alleina follow self
				mpforce alleina mpmod_qmark self forest_fiesta v0 = 8
				mpforce hawk mpmod_qmark self forest_fiesta v0 = 8
			endif
		endif
	endif
~
>talk_prog stop~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				mpechoaround $n $n ¶}©l¶}¤f½Õ°±¥L­Ìªº¯Éª§....¡C
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n [1;32m§A¦V $I ªí©ú¤F¦Û¤vÄ@·N¨ó§U ÀN§J ¦¨ªøªº·NÄ@¡C[0m
				gasp $n
				mpechoat $n $I »¡¡G·í¤@­Ó°V½mªº¦Ñ®v¬O¬Û·íÁcº¾²Ö¤Hªº¡A
				mpechoat $n $I »¡¡G±z¯uªºÄ@·N°V½m(train)³o¤£¦¨§÷ªº¤p¤l¡AÀ°§U¥L¦¨ªø¡H
			endif
		endif
	endif
~
>talk_prog start~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoaround $n [1;32m¥L­Ì¦n¹³ª§§n¤F°_¨Ó....¡I[0m
				mpechoat $n $I »¡¡G ÀN§J §A¨Ó°µ¤°»ò¡HÃø¹D§A¥h¨D $n À°¦£¡H
				mpechoat $n $I »¡¡G§AÃø¹D¤£ÁA¸Ñ§Aªº¸g¾ú¤´µM¤£°÷¶Ü¡H
				mpechoat $n $I »¡¡G³º´±¥h³Â·Ð§Ú­Ì¤g¤H³¡¸¨ªº®¦¤H....¡I
				mpforce alleina shake
				mpechoat $n ¦ã½¬®R(Alleina) »¡¡Gª¨ª¨¡A§A»~·|¤F¡A¬O§Ú§ä¨Óªº....¡C
				mpechoat $n [1;32m·í¥L­Ì§nªº¤£¥i¶}¥æ®É§A¤ß·Q¡G¬O§A¦Û¤v¸Ó¥X­±ªº®É­Ô¤F(stop)¡I[0m
			else
				mpechoat $n »·¤è¶Ç¨Ó¤@°}ÁnÅT¡G¤H¥¼¨ì»ô³á¡I
			endif
		else
			mpechoat $n »·¤è¶Ç¨Ó¤@°}ÁnÅT¡G¤H¥¼¨ì»ô³á¡I
		endif
	endif
~
>talk_prog quest~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 300063
		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		mpechoat $n $I »¡¡G§A¤w¸g§¹¦¨§Ú»Ý­n§A°µªº©Ò¦³¥ô°È....¡C
		break
	endif
	if is_qmark($n) == forest_quest 100002
		mpechoat $n $I »¡¡G¥H¤U¬O§Ú§Æ±æ§A¯à°÷§¹¦¨ªº¥ô°È¡G                                       
		if is_qmark($n) !& forest_quest 300002
			mpechoat $n $I »¡¡G   ¡E[1;36m¨ú±oªh¿A³D¦ZªºÅì¤ù(Scale of Snake Queen)[0m
		endif
		if is_qmark($n) !& forest_quest 300004
			mpechoat $n $I »¡¡G   ¡E[1;36m¨ú±o·à¤l¤§¤ýªº²´·ú(Eyes of Lion King)[0m
		endif
		if is_qmark($n) !& forest_quest 300008
			mpechoat $n $I »¡¡G   ¡E[1;36m¨ú±o¬}¥Þ¥¨¤H¤ýªº¥Ö½§(Skin of Troll King)[0m
		endif
		if is_qmark($n) !& forest_quest 300016
			mpechoat $n $I »¡¡G   ¡E[1;36m¨ú±o¯«¤ì¤§³»ªº¹àªÞ(Burgeon of Old Tree)[0m
		endif
		if is_qmark($n) !& forest_quest 300032
			mpechoat $n $I »¡¡G   ¡E[1;36m¨ú±o¥¨«¬¤õôÆë`ªº¤ßÅ¦(Heart of Fire Newt)[0m
		endif  
		mpechoat $n $I »¡¡G¥H¤W¨º¨ÇªF¦è¡A¥i¥HÀ°§U²½®v°µ¥X¥i¥H©è§Ü¤õÀs°ª¼öªºª««~¡C
		mpechoat $n $I »¡¡G½Ð§A§¹¦¨¥H¤Wªº¥ô°È¡A¥HÃÒ©ú§A¦³¸ê®æ«e©¹®ø·À¤õÀsÁÉ¤Ú¬¥¥q¡C
	endif
~
>talk_prog restart~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) == forest_quest 200001
			if is_qmark($n) == forest_scale 100000
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n $I »¡¡G§A©Ò±µªº[1;36m³D¦ZÅì¤ù[0m¥ô°È¤w¸g­«·s¶}©l¤F¡C
				mpdel_qmark $n forest_scale
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_eyes 100000
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n $I »¡¡G§A©Ò±µªº[1;36m·à¤ý¤§²´[0m¥ô°È¤w¸g­«·s¶}©l¤F¡C
				mpdel_qmark $n forest_eyes
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_skin 100000
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n $I »¡¡G§A©Ò±µªº[1;36m¥¨¤H¤ý¥Ö½§[0m¥ô°È¤w¸g­«·s¶}©l¤F¡C
				mpdel_qmark $n forest_skin
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) < forest_burgeon 100002
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n $I »¡¡G§A©Ò±µªº[1;36m¯«¤ì¹àªÞ[0m¥ô°È¤w¸g­«·s¶}©l¤F¡C
				mpdel_qmark $n forest_burgeon
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_heart 100000
				mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
				mpechoat $n $I »¡¡G§A©Ò±µªº[1;36m¤õôÆë`¤§¤ß[0m¥ô°È¤w¸g­«·s¶}©l¤F¡C
				mpdel_qmark $n forest_heart
				mpmod_qmark $n forest_quest v1 = 0
			endif
		endif
	endif
~	
>talk_prog assistant~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 100004
		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		mpmod_qmark $n forest_quest v0 + 1
		mpechoat $n $I »¡¡G¨S°ÝÃD¡A§Ú¥ß¨è¥s§Ú±Ú¨â­Ó¾Ô¤h«e¨ÓÀ°§U§A¡I
		mpecho $I ¤j³Ü¤@Án¡A¥ß¨è¥X²{¨â­Ó±j§§ªº¤g¤H¾Ô¤h¡C
		mpechoat $n [1;32m³o¨â­Óµ×­^¾Ô¤h¡A¥[¤J¤F§Aªº¶¤¥î¤§¤¤¡C[0m
		mpechoaround $n [1;32m³o¨â­Óµ×­^¾Ô¤h¡A¥[¤J¤F[0m $n [1;32mªº¶¤¥î¤§¤¤¡C[0m
		mpmload 4245
		mpforce native mpoload 4213
		mpforce native mpoload 4218
		mpforce native mpoload 4219
		mpforce native mpdef_rembch $n
		mpforce native mpat 5 wear all
		mpforce native mpgroup $n self 1 0
		mpmload 4245
		mpforce native mpoload 4213
		mpforce native mpoload 4220
		mpforce native mpoload 4223
		mpforce native mpdef_rembch $n
		mpforce native mpat 5 wear all
		mpforce native mpgroup $n self 1 0
		mpechoat $n $I »¡¡G»°ºò«e©¹¤õ¤s¤f¤¤®ø·À¥i´cªº ÁÉ¤Ú¬¥¥q §a¡I
		wave $n
	endif
~		
>talk_prog p heart of fire newt~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_heart
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¥¿¦b¶i¦æ¨ä¥Lªº¥ô°È¡C
			mpechoat $n $I »¡¡G½Ð±N§A¥ý«e±µªº¥ô°Èµ¹§¹¦¨¡C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_heart 100002
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¤£¬O¥¿¦b¶i¦æ³o¶µ¥ô°È¶Ü¡H
			mpechoat $n $I »¡¡G¦pªG§A·Q­n­«·s¨Ó¹L¡A¸ò§Ú»¡ restart ¡C
			break
		endif
		if is_qmark($n) != forest_heart
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			nod $n
			mpechoat $n $I »¡¡Gµ½©ó¼Q¤õªº¤õôÆë`¥Íªø¦bÆZ¯î¤õ¤sªº¤s¸y³B¡C
			mpechoat $n $I »¡¡G¦ý¬O¤p¤ß¤õ¤sªº°ª·Å·|¼vÅT§Aªº¥Í²zª¬ºA¡C
			mpechoat $n $I »¡¡G¦Ó¥Bªþªñ¤]¦³¬}¥Þ¥¨¤Hªº¥X¨S¡A¤£¥i¤£¨¾¡C
			mpechoat $n $I »¡¡G§A¥²¶·¡u¿Ë¦Û¡v§¹¦¨³o­Ó¥ô°È¡A§_«hµLªk³q¹L»{©w¡C
			wave $n
			mpadd_qmark $n forest_heart -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4248~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		give heart $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_heart 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 		clap $n
		 		mpechoat $n $I »¡¡G®¥³ß§A§¹¦¨¨ú±o[1;36m¤õôÆë`¤§¤ß(Heart of Fire Newt)[0mªº¥ô°È¡C
		 		mpmod_qmark $n forest_quest v2 + 32
		 		mpdel_qmark $n forest_heart
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk heart
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 			hop
		 			mpechoat $n $I »¡¡G²×©ó®³¨ì¤­¶µ»s§@[1;31m¤õµK¯«¯](Orb of Flame)[0mªº­ì®Æ¤F¡C
		 			mpechoat $n $I »¡¡G§Ú²{¦b¥ß¨è±N³o¨Ç­ì®Æ®³¥hµ¹²½®v¡A§A»°ºòÀH«á¸ò¨Ó¡C
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n §A¬Ý¨£ $I ¥Xªù©¹²½®v¦í³Bªº¤è¦V¨«¥h¡C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p burgeon of old tree~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_burgeon
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¥¿¦b¶i¦æ¨ä¥Lªº¥ô°È¡C
			mpechoat $n $I »¡¡G½Ð±N§A¥ý«e±µªº¥ô°Èµ¹§¹¦¨¡C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_burgeon 100003
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¤£¬O¥¿¦b¶i¦æ³o¶µ¥ô°È¶Ü¡H
			mpechoat $n $I »¡¡G¦pªG§A·Q­n­«·s¨Ó¹L¡A¸ò§Ú»¡ restart ¡C
			break
		endif
		if is_qmark($n) != forest_burgeon
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			nod $n
			mpechoat $n $I »¡¡Gªý¹jªF¤è¤j¨Fºz»PÆZ¯î¥­­ìªº¤¤¶¡¡A¦³­Ó¿@±Kªº´ËªL¦a±a¡C
			mpechoat $n $I »¡¡G´ËªLªº¤¤¥¡¡A¦³¤@´Ê¥¨¤jªº¯«¤ì¡C
			mpechoat $n $I »¡¡G½Ð§â¯«¤ì¦yºÝªº¹àªÞ¥æµ¹§Ú¡A¦ý¬O½Ð¤p¤ß¨º¸Ìªº¦ÑÆN­Ì¡C
			mpechoat $n $I »¡¡G§A¥²¶·¡u¿Ë¦Û¡v§¹¦¨³o­Ó¥ô°È¡A§_«hµLªk³q¹L»{©w¡C
			mpat 4245 mpoload 4247 floor
			wave $n
			mpadd_qmark $n forest_burgeon -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4247~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		give burgeon $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_burgeon 100002
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 		clap $n
		 		mpechoat $n $I »¡¡G®¥³ß§A§¹¦¨¨ú±o[1;36m¯«¤ì¤§³»ªº¹àªÞ(Burgeon of Old Tree)[0mªº¥ô°È¡C
		 		mpmod_qmark $n forest_quest v2 + 16
		 		mpdel_qmark $n forest_burgeon
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk burgeon
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 			hop
		 			mpechoat $n $I »¡¡G²×©ó®³¨ì¤­¶µ»s§@[1;31m¤õµK¯«¯](Orb of Flame)[0mªº­ì®Æ¤F¡C
		 			mpechoat $n $I »¡¡G§Ú²{¦b¥ß¨è±N³o¨Ç­ì®Æ®³¥hµ¹²½®v¡A§A»°ºòÀH«á¸ò¨Ó¡C
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n §A¬Ý¨£ $I ¥Xªù©¹²½®v¦í³Bªº¤è¦V¨«¥h¡C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p skin of troll king~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_skin
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¥¿¦b¶i¦æ¨ä¥Lªºªº¥ô°È¡C
			mpechoat $n $I »¡¡G½Ð±N§A¥ý«e±µªº¥ô°Èµ¹§¹¦¨¡C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_skin 100002
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¤£¬O¥¿¦b¶i¦æ³o¶µ¥ô°È¶Ü¡H
			mpechoat $n $I »¡¡G¦pªG§A·Q­n­«·s¨Ó¹L¡A¸ò§Ú»¡ restart ¡C
			break
		endif
		if is_qmark($n) != forest_skin
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			nod $n
			mpechoat $n $I »¡¡G¬}¥Þ¥¨¤Hªº±_¥Þ¦b¥_¤èÆZ¯î¤õ¤sªº¤s¸}¤U¡C
			mpechoat $n $I »¡¡G°£¤F¥¨¤H¤ý¤Q¤Àªº±j®«¤§¥~¡A¨ä¥Lªº¥¨¤H¤]·|§ðÀ»§A¡C
			mpechoat $n $I »¡¡G¬}¥Þ¥¨¤Hªº¤O¶q«D±`«D±`¤j¡A§Æ±æ§A¤d¸U¤p¤ß¡C
			mpechoat $n $I »¡¡G§A¥²¶·¡u¿Ë¦Û¡v§¹¦¨³o­Ó¥ô°È¡A§_«hµLªk³q¹L»{©w¡C
			wave $n
			mpadd_qmark $n forest_skin -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4246~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		give skin $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_skin 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 		clap $n
		 		mpechoat $n $I »¡¡G®¥³ß§A§¹¦¨¨ú±o[1;36m¬}¥Þ¥¨¤H¤ý¥Ö½§(Skin of Troll King)[0mªº¥ô°È¡C
		 		mpmod_qmark $n forest_quest v2 + 8
		 		mpdel_qmark $n forest_skin
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk skin
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 			hop
		 			mpechoat $n $I »¡¡G²×©ó®³¨ì¤­¶µ»s§@[1;31m¤õµK¯«¯](Orb of Flame)[0mªº­ì®Æ¤F¡C
		 			mpechoat $n $I »¡¡G§Ú²{¦b¥ß¨è±N³o¨Ç­ì®Æ®³¥hµ¹²½®v¡A§A»°ºòÀH«á¸ò¨Ó¡C
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n §A¬Ý¨£ $I ¥Xªù©¹²½®v¦í³Bªº¤è¦V¨«¥h¡C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p eyes of lion king~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_eyes
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¥¿¦b¶i¦æ¨ä¥Lªº¥ô°È¡C
			mpechoat $n $I »¡¡G½Ð±N§A¥ý«e±µªº¥ô°Èµ¹§¹¦¨¡C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_eyes 100002
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¤£¬O¥¿¦b¶i¦æ³o¶µ¥ô°È¶Ü¡H
			mpechoat $n $I »¡¡G¦pªG§A·Q­n­«·s¨Ó¹L¡A¸ò§Ú»¡ restart ¡C
			break
		endif
		if is_qmark($n) != forest_eyes
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			nod $n
			mpechoat $n $I »¡¡G·à¤l­Ìªº»â°ì¦bÆZ¯î¥­­ìªºªF«n¤è¡C
			mpechoat $n $I »¡¡G°£¤F·à¤ý¤Q¤Àªº±j®«¤§¥~¡A¨ä¥Lªº·à¤l¤]·|§ðÀ»§A¡C
			mpechoat $n $I »¡¡G¦p¨S¦³¬Û·í§â´¤¡A½Ð¥R¹ê¦Û¤vªº¹ê¤O¤§«á¦A¥h§¹¦¨¥ô°È¡C
			mpechoat $n
			mpechoat $n $I »¡¡G§A¥²¶·¡u¿Ë¦Û¡v§¹¦¨³o­Ó¥ô°È¡A§_«hµLªk³q¹L»{©w¡C
			wave $n
			mpadd_qmark $n forest_eyes -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4245~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		give eyes $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_eyes 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 		clap $n
		 		mpechoat $n $I »¡¡G®¥³ß§A§¹¦¨¨ú±o[1;36m·à¤ý²´·ú(Eyes of Lion King)[0mªº¥ô°È¡C
		 		mpmod_qmark $n forest_quest v2 + 4
		 		mpdel_qmark $n forest_eyes
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk eyes
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 			hop
		 			mpechoat $n $I »¡¡G²×©ó®³¨ì¤­¶µ»s§@[1;31m¤õµK¯«¯](Orb of Flame)[0mªº­ì®Æ¤F¡C
		 			mpechoat $n $I »¡¡G§Ú²{¦b¥ß¨è±N³o¨Ç­ì®Æ®³¥hµ¹²½®v¡A§A»°ºòÀH«á¸ò¨Ó¡C
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n §A¬Ý¨£ $I ¥Xªù©¹²½®v¦í³Bªº¤è¦V¨«¥h¡C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p scale of snake queen~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_scale
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¥¿¦b¶i¦æ¨ä¥Lªº¥ô°È¡C
			mpechoat $n $I »¡¡G½Ð±N§A¥ý«e±µªº¥ô°Èµ¹§¹¦¨¡C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_scale 100002
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			mpechoat $n $I »¡¡G§A²{¦b¤£¬O¥¿¦b¶i¦æ³o¶µ¥ô°È¶Ü¡H
			mpechoat $n $I »¡¡G¦pªG§A·Q­n­«·s¨Ó¹L¡A¸ò§Ú»¡ restart ¡C
			break
		endif
		if is_qmark($n) != forest_scale
			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
			nod $n
			mpechoat $n $I »¡¡G³D¦Z¦bÆZ¯î¥­­ìªºªF¤èªh¿A°Ïªº³Ì¸Ì­±¡C
			mpechoat $n $I »¡¡G¾ã­Óªh¿A°Ïªºªh®ð»P«I»k©Êªºªh¿A¤ô·|¶Ë®`§Aªº¥Ö½§¡C
			mpechoat $n $I »¡¡G¦]¦¹½Ð¦h¥[¤p¤ß¡C
			mpechoat $n $I »¡¡G§A¥²¶·¡u¿Ë¦Û¡v§¹¦¨³o­Ó¥ô°È¡A§_«hµLªk³q¹L»{©w¡C
			wave $n
			mpadd_qmark $n forest_scale -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4244~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I »¡¡G©êºp¡A²{¦b¥¿¬OÂ×¦~²½ªº­«­n¤é¤l¡C
		mpechoat $n $I »¡¡G¦³¤°»ò¨Æ½Ð«ÝÂ×¦~²½µ²§ô¤§«á¦A»¡¡C
		give scale $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_scale 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 		clap $n
		 		mpechoat $n $I »¡¡G®¥³ß§A§¹¦¨¨ú±o[1;36m³D¦ZÅì¤ù(Scale of Snake Queen)[0mªº¥ô°È¡C
		 		mpmod_qmark $n forest_quest v2 + 2
		 		mpdel_qmark $n forest_scale
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk scale
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ¥¿¥Î¼ô½mªº¤g»y»P $I »¡¸Ü....¡C
		 			hop
		 			mpechoat $n $I »¡¡G²×©ó®³¨ì¤­¶µ»s§@[1;31m¤õµK¯«¯](Orb of Flame)[0mªº­ì®Æ¤F¡C
		 			mpechoat $n $I »¡¡G§Ú²{¦b¥ß¨è±N³o¨Ç­ì®Æ®³¥hµ¹²½®v¡A§A»°ºòÀH«á¸ò¨Ó¡C
		 			mpmod_qmark $n forest_quest v0 + 1
		 			open e
		 			close e
		 			mpecho §A¬Ý¨£ $I ¥Xªù©¹²½®v¦í³Bªº¤è¦V¨«¥h¡C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>fight_prog 60~
	if insameroom(alleina)
		mpgroup self alleina 1 0
		protect alleina
		break
	endif
	if isaffected($i) !& 128
		mpat 5 c sanctuary
		break
	endif
	if isaffected($i) & 1
		mpat c 'cure blindness'
	endif
~
>fight_prog 40~
	mpat 5 c heal
	wear sword
	wear sword
	disarm
	if rand(30)
		coh
		break
	endif
	if rand(30)
		frenzy
		break
	endif
	if rand(10)
		mpat 5 invoke
	else
		c blind
		c heal
		hunt $r
	endif
~
>death_prog 50~
	mpdis_magic self 1
	mpat 5 c inf
	if is_qmark($n) != forest_enemy
		mpadd_qmark $n forest_enemy 50 0 0 0 0 1
	else
		mpmod_qmark $n forest_enemy timer + 50
	endif
~
>tick_prog 100~
	if is_qmark($i) == forest_fiesta 300000
		mpmod_qmark self forest_fiesta v1 + 1
		if is_qmark($i) == forest_fiesta 200012
			mpmod_qmark self forest_fiesta v0 = 1
			mpforce alleina mpmod_qmark self forest_fiesta v0 + 1			break
		endif
		if is_qmark($i) == forest_fiesta 200024
			mpmod_qmark self forest_fiesta v0 = 2
			mpforce alleina mpmod_qmark self forest_fiesta v0 + 1
			mpecho $I [1;32m©¿µM¤j³Ü¤@Án¡A±µµÛ¦b¼s³õ¤¤¥¡¸÷¤F¤U¨Ó....¡C[0m
			mpecho $I [1;32m¤£Â_ªº¦V¼s³õ¤¤¥¡ªº¯«¹³³ä³ä°áµÛÅ¥¤£À´ªº¸Ü¡C[0m
			break
		endif
		if is_qmark($i) > forest_fiesta 200024
			if is_qmark($i) < forest_fiesta 200048
				if rand(30)
					mpecho $I [1;32m¤£Â_ªº¦V¼s³õ¤¤¥¡¯«¹³»¡¨Ç§AÅ¥¤£À´ªº¸Ü¡C[0m
					break
				endif
				if rand(30)
					mpecho ¯Á¤Ú(Thorba) [1;32m¦b¦a¤WºCºCª¦¦æ±µªñ¯«¹³¡A¦n¹³¥Nªí¤°»ò¯S®í·N¸q¡C[0m
					break
				endif
				if rand(30)
					mpecho ¯Á¤Ú(Thorba) [1;32m¶}©l©À¤@¨Ç©G»y¡A¦n¹³¬O¬è¨D¥­¦wªº¡C[0m
				endif
			endif
			break
		endif
		if is_qmark($i) == forest_fiesta 200048
			mpecho $I [1;32m¦ü¥G¤£Ä~Äò¦V¼s³õ¤¤¥¡¯«¹³°Ñ«ô¤F¡C[0m
			mpecho $I [1;32m°_¨­µ²§ô¬èÃ«»ö¦¡¡AÂ×¦~²½ªº¼ö¾x®ðª^¤S¦A«×¶}©l¤F¡C[0m
			break
		endif
		if is_qmark($i) == forest_fiesta 200072
			mpdel_qmark self forest_fiesta
			rumor [1;32m¾Ú»DÆZ¯î´ËªL¤g¤HÂ×¦~²½¤w¸gµ²§ô¤F¡C[0m
			mptransfer thorba 4304
			mpgoto 4203
			mppurge feetsteps
			mpgoto 4206
			mppurge feetsteps
			mpgoto 4209
			mppurge feetsteps
			mpgoto 4224
			if insameroom(hawk)
				mppurge hawk
			endif
			if insameroom(alleina)
				mpforce alleina mpdel_qmark self forest_fiesta
				mptransfer alleina 4296
			endif
			mpgoto 4296
		endif
		break
	endif
	if month($) == 0
	or month($) == 6
	or month($) == 12
		if day($) == 1
			if hour($) == 6
				rumor [1;32m¾Ú»D¤»­Ó¤ë¤@«×ªºÆZ¯î´ËªL¤g¤HÂ×¦~²½§Y±N¶}©l¡C[0m
				endif
			break
			endif
		endif
		if day($) == 18
			if is_qmark($i) != forest_fiesta
				rumor [1;32m¾Ú»DÆZ¯î´ËªL¤g¤HÂ×¦~²½¤w¸g¶}©l¤F¡C[0m
				mpat 4304 mpforce thorba mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpforce alleina mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpforce alleina mpgoto 4297
				mpat 4304 mpforce thorba mpgoto 4297
				mpgoto 4297
				mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
			endif
			break
		endif
	endif
	if hour($) > 5
		if hour($) < 7
			wak
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4305
					mpecho $I °_§É¦ù¤F¦ùÃi¸y¡A¦ü¥G­n¥h¨µµø¤F¡C
					open e
					e
					close w
					e
					e
				else
					mpgoto 4305
					mpecho $I °_§É¦ù¤F¦ùÃi¸y¡A¦ü¥G­n¥h¨µµø¤F¡C
					open e
					e
					close w
					e
					e
				endif
			endif
		endif
		break
	endif
	if hour($) > 12
		if hour($) < 14
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4296
					mpecho $I ÂI¤FÂIÀY¡AµM«á©¹¦Û¤vªº¦í³B¨«¥h¡C
					w
					w
					open w
					w
					close e
					rest
				else
					mpgoto 4296
					mpecho $I ÂI¤FÂIÀY¡AµM«á©¹¦Û¤vªº¦í³B¨«¥h¡C
					w
					w
					open w
					w
					close e
					rest
				endif
			endif
		endif
		break
	endif
	if hour($) > 0
		if hour($) < 2
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4305
					mpecho $I ¥´¤F¤@­Ó«¢¤í¡AµM«á­ËÀY«KºÎ¡C
					sleep
				else
					mpgoto 4305
					mpecho $I ¥´¤F¤@­Ó«¢¤í¡AµM«á­ËÀY«KºÎ¡C
					sleep
				endif
			endif
		endif
	endif
~
|