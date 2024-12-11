>tick_prog 100~
if being($b)
	break
else
	mppurge self
endif
~
>repop_prog 100~
mpadd_qmark self statue_hurt -1 0 0 0 0 1
~
>hitprcnt_prog 50~
mpat 5 c 'full heal'
mpgoto self
~
>fight_prog 100~
mpgoto self
~
>hurt_prog 43~
mpat $b mpecho $Nªº¤õ²y³N¥´±o§A[1;33m²r¼QÂA¦å¡A¾¦¤ú°nµõ[m.(601-700)
mpechoat $n §Aªº¤õ²y³NÅý¦B¹³¶}©l¤j¶q·»¤Æ¤F
mpechoaround $n $Nªº¤õ²y³NÅý¦B¹³¶}©l¤j¶q·»¤Æ¤F
mpat $b mpforcer here modhmv pc 01.601 700 0
mpmod_qmark self statue_hurt v0 + 5
if is_qmark($i) >= statue_hurt 100015
	mpecho $B±q·»¤Æªº¦BÀJ¤¤¨«¤F¥X¨Ó
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 45~
mpat $b mpecho $Nªº¬P¬P¤§¤õ¥´±o§A[1;31m¡Iµ¬-¯ß-¤o-Â_¡A ¦å-¦×-¼Ò-½k¡I[m(1151-1300)
mpechoat $n §Aªº¬P¬P¤§¤õÅý¦B¹³¶}©l¤j¶q·»¤Æ¤F
mpechoaround $n $Nªº¬P¬P¤§¤õÅý¦B¹³¶}©l¤j¶q·»¤Æ¤F
mpat $b mpforcer here modhmv pc 01.1151 1300 0
mpmod_qmark self statue_hurt v0 + 7
if is_qmark($i) >= statue_hurt 100015
	mpecho $B±q·»¤Æªº¦BÀJ¤¤¨«¤F¥X¨Ó
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 7~
mpat $b mpecho $Nªº¨`¼ö¤§´x[1;36mÀ~¨ì[m(13-18)§A.
mpechoat $n §Aªº¨`¼ö¤§´xÅý¦B¹³¶}©l·»¤Æ¤F
mpechoaround $n $Nªº¨`¼ö¤§´xÅý¦B¹³¶}©l·»¤Æ¤F
mpat $b mpforcer here modhmv pc 01.13 18 0
mpmod_qmark self statue_hurt v0 + 1
if is_qmark($i) >= statue_hurt 100015
	mpecho $B±q·»¤Æªº¦BÀJ¤¤¨«¤F¥X¨Ó
	mptransfer $b
	mppurge self
endif
~
>hurt_prog 46~
mpat $b mpecho $Nªº¸t¤õÀ»¥´±o§A[1;35m¯Q«C·ï¦å[m.(61-85)
mpechoat $n §Aªº¸t¤õÀ»Åý¦B¹³¶}©l·»¤Æ¤F
mpechoaround $n $Nªº¸t¤õÀ»Åý¦B¹³¶}©l·»¤Æ¤F
mpat $b mpforcer here modhmv pc 01.61 85 0
mpmod_qmark self statue_hurt v0 + 3
if is_qmark($i) >= statue_hurt 100015
	mpecho $B±q·»¤Æªº¦BÀJ¤¤¨«¤F¥X¨Ó
	mptransfer $b
	mppurge self
endif
~
>death_prog 100~
mpecho $I³Q¥~¤OÀ»¸H¤F.
mppurge self
~
|