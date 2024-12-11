>death_prog 100~
if ispc($n)
    yell $N¬O¤@­Ó±þ¤H¥û¤â¡A¤j®a­n¬°§Ú³ø¤³«z¡I
    mpdis_magic self all
    mpat 5 c infr
    mpadd_qmark $n hobbitcity_killer 50 1 0 0 0 1
endif
~
>fight_prog 20~
if mobcount(14006) < 10
yell ¨Ó¤H°Ú¡I$N¦b³o¸Ì³x¥û°Ú¡I
mpecho [36m¤@¦ìªv¦wÄµ½Ã(security guard)Å¥¨ì³Û¥s«æ¦£»°¨Ó¡I[0m
mpmload 14006
mpforce security protect $i
endif
~
>greet_prog 50~
if is_qmark($n) == hobbitcity_killer
yell Äµ½Ã§Ö¨Ó¡I±þ¤H¥Ç$N¦b³o¸Ì¡I
mpecho [36m¤@¦ìªv¦wÄµ½Ã(security guard)¥^¥^¦£¦£¶]¨Ó¡I[0m
mpmload 14006
mpforce security say ¦n¤p¤l¡AÁÙ¤£§ô¤â´N¾à¡C
mpforce security mpkill $n
endif
~
|