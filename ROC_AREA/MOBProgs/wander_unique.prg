>defun_prog wander_unique_reborn~
mpadd_qmark self wander_unique -1 0 0 0 0 1
if rand(10)
	mpmod_qmark self wander_unique v0 | 1
else
	if rand(11)
		mpmod_qmark self wander_unique v0 | 2
	else
		if rand(12)
			mpmod_qmark self wander_unique v0 | 4
		else
			if rand(14)
				mpmod_qmark self wander_unique v0 | 8
			else
				if rand(17)
					mpmod_qmark self wander_unique v0 | 16
				else
					if rand(20)
						mpmod_qmark self wander_unique v0 | 32
					else
						if rand(25)
							mpmod_qmark self wander_unique v0 | 64
						else
							if rand(33)
								mpmod_qmark self wander_unique v0 | 128
							else
								if rand(50)
									mpmod_qmark self wander_unique v0 | 256
								else
									mpmod_qmark self wander_unique v0 | 512
								endif
							endif
						endif
					endif
				endif
			endif
		endif
	endif
endif
~
>defun_prog wander_unique_ability~
if is_qmark($i) & wander_unique 100001
	mpmodify dr + 100
	mpmodify apdr + 200
endif
if is_qmark($i) & wander_unique 100002
	mpmodify hr + 100
	mpmodify dc - 500
endif
if is_qmark($i) & wander_unique 100004
	mpforcer here c 'mystic armor' $i 60
endif
if is_qmark($i) & wander_unique 100008
	mpforcer here c 'mystic armor' $i 60
	mpmodify act | 16777216
endif
if is_qmark($i) & wander_unique 100016
	mpmodify ac - 5000
	mpmodify pdr + 100
endif
if is_qmark($i) & wander_unique 100032
	mpmodify mdr + 100
	mpmodify amdr + 100
endif
if is_qmark($i) & wander_unique 100064
	mpmodify mdr + 100
	mpmodify amdr + 100
endif
if is_qmark($i) & wander_unique 100128
	mpmodify mdr + 100
	mpmodify amdr + 100
endif
if is_qmark($i) & wander_unique 100256
	mpmodify special | 512
	mpmodify amdr + 100
endif
if is_qmark($i) & wander_unique 100512
	mpmodify mdr + 50
	mpmodify amdr + 100
endif
~
>defun_prog wander_unique_lore~
mpechoat $n $I©Ò¾Ö¦³ªº²§¯à¡G
if is_qmark($i) & wander_unique 100001
	mpechoat $n [1;33m¡E¯S§O±j§§[m
endif
if is_qmark($i) & wander_unique 100002
	mpechoat $n [1;32m¡E¯S§O¨³³t[m
endif
if is_qmark($i) & wander_unique 100004
	mpechoat $n [1;34m¡Eªk¤O¿U¿N[m
endif
if is_qmark($i) & wander_unique 100008
	mpechoat $n [1;30m¡E¨¸ÆF¤§Ä²[m
endif
if is_qmark($i) & wander_unique 100016
	mpechoat $n [33m¡E»É¥ÖÅK°©[m
endif
if is_qmark($i) & wander_unique 100032
	mpechoat $n [1;31m¡E¤õµK±j¤Æ[m
endif
if is_qmark($i) & wander_unique 100064
	mpechoat $n [1;36m¡E¦B§N±j¤Æ[m
endif
if is_qmark($i) & wander_unique 100128
	mpechoat $n [1;37m¡E°{¹q±j¤Æ[m
endif
if is_qmark($i) & wander_unique 100256
	mpechoat $n [1;35m¡EÅ]ªk§Ü©Ê[m
endif
if is_qmark($i) & wander_unique 100512
	mpechoat $n [1;33m¡E¤j¦a±j¤Æ[m
endif
~
>defun_prog wander_unique_fight~
if is_qmark($i) & wander_unique 100001
	skilldrv heavyhit
	skilldrv heavyhit
	skilldrv heavyhit
	skilldrv heavyhit
	skilldrv heavyhit
	skilldrv heavyhit
endif
if is_qmark($i) & wander_unique 100002
	skilldrv speedhit
	skilldrv speedhit
	skilldrv speedhit
	skilldrv speedhit
	skilldrv speedhit
	skilldrv speedhit
endif
if is_qmark($i) & wander_unique 100004
	if rand(50)
		mpforcer here c 'mystic armor' $i 60
	endif
	mpechoat $n §A·P¨ì§AªºÅ]¤O³vº¥¬y¥¢¡C
	mpforcer here modhmv $n 11.250 500 00000000
	mpechoat $r §A·P¨ì§AªºÅ]¤O³vº¥¬y¥¢¡C
	mpforcer here modhmv $r 11.250 500 00000000
endif
if is_qmark($i) & wander_unique 100008
	mpecho $I[1;7;30;47m¨­Åé©P³òµo¥X¶Â¦âªº¥ú¨~....[m
	mpforcer here c armageddon $n 60
	mpnowait
	stand
	mpdel_qmark self spell_armageddon_timer
	mpforcer here c armageddon $r 60
	mpnowait
	stand
	mpdel_qmark self spell_armageddon_timer
endif
if is_qmark($i) & wander_unique 100032
	mpecho $I[1;31m³òÂ¶¤F¤@¹Î¤õµK....[m
	if rand(50)
		mpforcer here c 'flam armor' $i 60
	endif
	mpforcer here c 4 fireball $n 60
	mpnowait
	mpforcer here c 4 fireball $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100064
	mpecho $I[1;36mµo¥X°}°}´H®ð....[m
	mpforcer here c 4 colour $n 60
	mpnowait
	mpforcer here c 4 colour $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100128
	mpecho $I[1;37mµo¥X°}°}ªº¹p¥ú....[m
	mpforcer here c 5 'ligh bolt' $n 60
	mpnowait
	mpforcer here c 5 'ligh bolt' $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100256
	mpecho $I[1;37m¤f¤¤¦R¥X¤»Áûºñ¦âªº¥ú²y....[m
	mpforcer here c 3 powerball $n 60
	mpnowait
	mpforcer here c 3 powerball $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100512
	mpecho $I[33m¶g³òÄjº©¤@ªÑ»Ä¨ý....[m
	mpforcer here c 6 'acid blast' $n 60
	mpnowait
	mpforcer here c 6 'acid blast' $r 60
	mpnowait
endif
~
>defun_prog wander_unique_death~
mpgreward skexp $n +36 500
mpforcer 59100 modrvt t0 = 200
if is_qmark($i) & wander_unique 100032
	mpecho [1;31mÀHµÛ$I[1;31mªº¦º¤`¡A¤@ªÑ±j¤jªº¤õµK¤O¶q³QÄÀ©ñ¤F¥X¨Ó¡C[m
	mpforcer here modhmv pc 01.500 750 00001150
	mpforcer here c 4 fireball $n 60
	mpnowait
	mpforcer here c 4 fireball $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100064
	mpecho [1;36mÀHµÛ$I[1;36mªº¦º¤`¡A¤@ªÑ·¥´Hªº¦B§N¤O¶q³QÄÀ©ñ¤F¥X¨Ó¡C[m
	mpforcer here modhmv pc 01.500 750 00001250
	mpforcer here c 4 colour $n 60
	mpnowait
	mpforcer here c 4 colour $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100128
	mpecho [1;37mÀHµÛ$I[1;37mªº¦º¤`¡A¤@ªÑ±j«lªº°{¹q¤O¶q³QÄÀ©ñ¤F¥X¨Ó¡C[m
	mpforcer here modhmv pc 01.500 750 00001750
	mpforcer here c 5 'ligh bolt' $n 60
	mpnowait
	mpforcer here c 5 'ligh bolt' $r 60
	mpnowait
endif
if is_qmark($i) & wander_unique 100512
	mpecho [33mÀHµÛ$I[33mªº¦º¤`¡A¤@ªÑ¶¯«pªº¤j¦a¤O¶q³QÄÀ©ñ¤F¥X¨Ó¡C[m
	mpforcer here modhmv pc 01.500 750 00001450
	mpforcer here c 6 'acid blast' $n 60
	mpnowait
	mpforcer here c 6 'acid blast' $r 60
	mpnowait
endif
if rand(13)
	if rand(60)
		mpoload 59248 40
	endif
	break
endif
if rand(14)
	if rand(60)
		mpoload 59249 40
	endif
	break
endif
if rand(17)
	if rand(60)
		mpoload 59250 40
	endif
	break
endif
if rand(20)
	if rand(60)
		mpoload 59251 40
	endif
	break
endif
if rand(25)
	if rand(60)
		mpoload 59252 40
	endif
	break
endif
if rand(33)
	if rand(60)
		mpoload 59253 40
	endif
	break
endif
if rand(50)
	if rand(60)
		mpoload 59254 40
	endif
else
	if rand(60)
		mpoload 59255 40
	endif
endif
~
|
