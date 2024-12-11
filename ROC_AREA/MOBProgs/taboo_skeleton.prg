>fight_prog 60~
mpdis_magic self all
mpecho $I ªº¨­Âß³Q¶Â·tªºÆF®ð³òÂ¶¡I
mpechoat $n $I [1;36m»a¥Õªº¤â°©«ö¦b§A¨­¤W¡I[m
if is_qmark($n) == taboo_skeleton 100005
	mpechoat $n $I ªº[1;36;44m´H¦B¤§Ä²[1;36m§¹¥þ[1;31m­áµ²[m¤F§A¡I
	mpechoat $n [1;36m§A¤w¸g¦¨¤F¦B§Nªº«Í¶ô...¨S¦³·PÄ±...¨S¦³ª¾Ä±...[m
	mpforcer here modhmv $n 03.100 100 0
	mpechoaround $n $I ªº[1;36;44m´H¦B¤§Ä²[m§¹¥þ­áµ²¤F $n ¡I
	c 'chill touch' $n
	break
endif
c 'chill touch' $n
mpnowait
if is_qmark($n) == taboo_skeleton 100004
	mpmod_qmark $n taboo_skeleton v0 = 5
	mpechoat $n [1;36m§AÄ±±o¦Û¤v´ý¨­³£¦B§N»øµw¡A³s¤ßÅ¦³£§Ö¤£¦æ¤F¡I[m
	break
endif
if is_qmark($n) == taboo_skeleton 100003
	mpmod_qmark $n taboo_skeleton v0 = 4
	mpechoat $n [1;36m§AÄ±±o¦Û¤v¥|ªÏº¥º¥»øµw°_¨Ó¡I[m
	break
endif
if is_qmark($n) == taboo_skeleton 100002
	mpmod_qmark $n taboo_skeleton v0 = 3
	mpechoat $n [1;36m§AÄ±±o¦Û¤vÂù¸}º¥º¥¥¢¥hª¾Ä±...[m
	break
endif
if is_qmark($n) == taboo_skeleton 100001
	mpmod_qmark $n taboo_skeleton v0 = 2
	mpechoat $n [1;36m§AÄ±±o¸}¦y¦³ÂI¦B§N...[m
	break
endif
if is_qmark($n) == taboo_skeleton 100000
	mpmod_qmark $n taboo_skeleton v0 = 1
	mpechoat $n [1;36m§AÄ±±o¦³ÂI¦B§N...[m
	break
endif
mpadd_qmark $n taboo_skeleton 6 0 0 0 0 1
mpechoaround $n $I ªº¾uÅ\¤â¦b $n ªº¨­¤W§ì¤F¤@¤U¡A$n ¦ü¥G·P¨ì¤Q¤Àµh­W¡I
~
>death_prog 100~
if class($n) == 0
or class($n) == 5
or class($n) == 6
or class($n) == 7
or class($n) == 8
or class($n) == 15
or class($n) == 16
	mpecho $I ¸H¦¨¯»¥½­¸³u......
	mpgreward exp $n -35 100
	mpgreward exp $n 36 50
	mpat 5 mppurge self
	break
else
	if class($n) == 14
	or class($n) == 18
	or class($n) == 19
		if rand(30)
			mpechoat $n $I ªºÀY°©³Q§A[1;31m­«­«¤@À»[mµ¹¥´¸H¤F¡I
			mpechoaround $n $I ªºÀY°©³Q $n [1;31m²r¯P¤@À»[mµ¹¥´Äê¤F¡I
			mpecho $I ¹³¬OÂ_¤F½uªº³ÈÀw´²¸¨¤@¦a...
			mpgreward exp $n -35 200
			mpgreward exp $n 36 100
			if rand(5)
				mpoload 32137
			endif
			mpat 5 mppurge self
			break
		endif
	endif
	if rand(5)
		mpecho $I ¦b¥¨¤O¤§¤U¸H¦¨¯»¥½­¸³u¡I
		mpat 5 mppurge self
		break
	endif
	mpecho $I ³Q­«­«¤@À»¥´Ãz¶}¨Ó¡I
	mpecho $I ªº¸H°©¥|´²¡IªÅ®ð¤¤¥Rº¡¤F¶Â¦âªº¨¸®ð¡I
	mpforcer here modhmv pc 03.9 10 0
	mpforcer here modhmv pc 11.100 200 0
	mpecho §A³Q¶Â¦âªº¨¸®ð¶æ±oÀY©ü²´ªá¡B¥|ªÏµL¤O......
	mpecho ´N·í§A·P¨ìÅé¤O³à¥¢ªºÀþ¶¡¡A§Aµo²{¸H°©­«²Õ¤F°_¨Ó¡I
	if rand(25)
		mpmload 32067
		mpecho $I ªº¯á´Õ°©±¼¨ì¤F¦a¤W¡I
		mpecho $I ªº²´¸]µo¥X¬õ¥ú¡I¦ü¥G«Ü¤£²n¡H¡I
		mpecho $I ±¼¨ì¦a¤Wªº¯á´Õ°©§l¦¬ªþªñªº¸H°©¦¨¤F¸Þ²§ªº§Îª¬¡I
		mpmload 32068
		mpforce fido-fr yell §q¶ã¡ã¡ã¡ã¡ã¡ã¶ã¡ã¡ã¡ã¡ã¶ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã
	else
		mpmload 32067
	endif
	mpforce communution-fr get all
	mpforce comminution-fr mpkill $n
	yell ¡I¡IOn Dead Road¡I¡ILet me Die¡I¡I
	mpat 5 mppurge self
endif
~
>repop_prog 100~
mpadd_qmark self delayed_cast -1 99999 0 0 1 0
mpadd_qmark self taboo_skeleton_hurt 6 0 0 0 0 1
~
>hurt_prog 92~
mpnowait
mpecho $I ªº¯á´Õ°©±¼¨ì¤F¦a¤W¡I
mpecho $I ªº²´¸]µo¥X¬õ¥ú¡I¦ü¥G«Ü¤£²n¡H¡I
mpecho $I ±¼¨ì¦a¤Wªº¯á´Õ°©§l¦¬ªþªñªº¸H°©¦¨¤F¸Þ²§ªº§Îª¬¡I
mpmload 32068
mpforce fido-fr yell §q¶ã¡ã¡ã¡ã¡ã¡ã¶ã¡ã¡ã¡ã¡ã¶ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã
~
>hurt_prog 1005~
if rand(50)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 108~
if rand(60)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 138~
if rand(60)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1000~
if rand(55)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1006~
if rand(40)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1007~
if rand(45)
	mpcall taboo_skeleton_load
endif
~
>hurt_prog 1008~
if rand(50)
	mpcall taboo_skeleton_load
endif
~
>defun_prog taboo_skeleton_load~
if is_qmark($i) != taboo_skeleton_hurt
	mpadd_qmark self taboo_skeleton_hurt 6 0 0 0 0 1
endif
if is_qmark($i) == taboo_skeleton_hurt 100005
	mpecho $I [1;30;47m¸Hµõ¦b¦a¡A¦¨¤F­ì¥»ªº¸H°©[m......
	if rand(40)
		mpechoat $n ¤@¤ù¸H°©¼u¨ì§A¨­¤W
		mpoload 32137 $n viclv target
	endif
	mpgreward exp $n -35 300
	mpgreward exp $n 36 200
	mpat 5 mppurge self
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100004
	mpmod_qmark self taboo_skeleton_hurt v0 = 5
	mpecho $I [1;30;47mªí­±§Gº¡¤F¦UºØ¸Hµõªº¯¾¸ô¡A¬Ý¨Ó§Ö­n´²¤F¡I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100003
	mpmod_qmark self taboo_skeleton_hurt v0 = 4
	mpecho $I [1;30;47mªºÀY°©¤]µo¥X¤F¸HµõªºÁn­µ¡I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100002
	mpmod_qmark self taboo_skeleton_hurt v0 = 3
	mpecho $I [1;30;47mªº»L°©µo¥X¸HµõªºÁn­µ¡I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100001
	mpmod_qmark self taboo_skeleton_hurt v0 = 2
	mpecho $I [1;30;47mªº¯Ý°©¤W¸Hµõ¤F¤@¶ô¡I[m
	break
endif
if is_qmark($i) == taboo_skeleton_hurt 100000
	mpmod_qmark self taboo_skeleton_hurt v0 = 1
	mpecho $I [1;30;47mªºªí­±¥X²{¤F¤@ÂIµõ²ª¡I[m
	break
endif
~
|