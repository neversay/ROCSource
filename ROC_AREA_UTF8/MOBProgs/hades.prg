>greet_prog 95~
if isnpc($n)
	break
else
	if isimmort($n)
		say Hello! my god friend.
	else
		say Let's see in the hell!!!
		yell POWER!!! POWER!!! POWER!!! Hahahah...
		c 3 powerball $n
	endif
endif
~
>fight_prog 20~
cast powerball
~
>fight_prog 15~
cast 'seal magic'
~
>fight_prog 15~
frenzy
~
>fight_prog 10~
cast 'full heal'
~
>fight_prog 80~
if isaffected($i) & 1
	cast 'cure blindness'
endif
if hitprcnt($i) < 60
	mpat 5 cast heal
endif
if rand(30)
	mpmload 910
	mpgroup self soldier 1 0
endif
~
>death_prog 100~
mpdis_magic self 1
mplog killed by $n
~
|
