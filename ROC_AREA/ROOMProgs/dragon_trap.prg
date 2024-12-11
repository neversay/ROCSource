>room_time 10~
if roomvalue($0) == 1
	modrvt v0 = 0
endif
~
>room_command disable trap~
echoat $n §A¹Á¸Õ¸Ñ°£³o¶¡©Ð¶¡¤ºªº³´¨À....
echoaround $n $n ¹Á¸Õ¸Ñ°£³o¶¡©Ð¶¡¤ºªº³´¨À....
if roomvalue($0) == 0
	if class($n) == 10
		if int($n) <= 15
			if rand(20)
				callout 2 echoat $n [1;36m§A¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À¡I¡C[m
				callout 2 echoaround $n $n [1;36m¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À....¡C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m§A¥¢±Ñ¤F¡I[m
				callout 2 echoaround $n $n [1;31m¸Ñ°£©Ð¶¡¤ºªº³´¨À¥¢±Ñ¤F¡I[m
				if rand(60)
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 17
			if rand(40)
				callout 2 echoat $n [1;36m§A¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À¡I¡C[m
				callout 2 echoaround $n $n [1;36m¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À....¡C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m§A¥¢±Ñ¤F¡I[m
				callout 2 echoaround $n $n [1;31m¸Ñ°£©Ð¶¡¤ºªº³´¨À¥¢±Ñ¤F¡I[m
				if rand(40)
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 19
			if rand(60)
				callout 2 echoat $n [1;36m§A¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À¡I¡C[m
				callout 2 echoaround $n $n [1;36m¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À....¡C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m§A¥¢±Ñ¤F¡I[m
				callout 2 echoaround $n $n [1;31m¸Ñ°£©Ð¶¡¤ºªº³´¨À¥¢±Ñ¤F¡I[m
				if rand(20)
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 21
			if rand(80)
				callout 2 echoat $n [1;36m§A¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À¡I¡C[m
				callout 2 echoaround $n $n [1;36m¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À....¡C[m
				callout 2 modrvt v0 = 1
			else
				callout 2 echoat $n [1;31m§A¥¢±Ñ¤F¡I[m
				callout 2 echoaround $n $n [1;31m¸Ñ°£©Ð¶¡¤ºªº³´¨À¥¢±Ñ¤F¡I[m
				if rand(10)
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 23
			callout 2 echoat $n [1;36m§A¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À¡I¡C[m
			callout 2 echoaround $n $n [1;36m¦¨¥\ªº¸Ñ°£¤F©Ð¶¡¤§¤ºªº³´¨À....¡C[m
			callout 2 modrvt v0 = 1
		endif
	else
		callout 1 echoat $n [1;33m§AªºÂ¾·~¤£¬O§ÔªÌ¡A¨Ã¤£·|¸Ñ°£©Ð¶¡³´¨À....¡C[m
		callout 1 echoaround $n $n [1;33m¹Á¸Õ¸Ñ°£©Ð¶¡ªº³´¨À¡A¦ý¬O¥¢±Ñ¤F....¡C[m
		callout 2 echoat $n [1;33m§AÄ²°Ê¤F©Ð¶¡¤ºªº³´¨À....¡C[m
		callout 2 echoaround $n $n [1;33mÄ²°Ê¤F©Ð¶¡¤ºªº³´¨À....¡C[m
		callout 2 call dragon_trap $n
	endif
else
	if rand(50)
		echo [1;36m³o¶¡©Ð¶¡ªº³´¨À¤w¸g³Q¸Ñ°£¤F....¡C[m
	else
		echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
	endif
endif
~
>room_command search around~
echoat $n §A¹Á¸Õ·j¯Á³o¶¡©Ð¶¡....
echoaround $n $n ¹Á¸Õ·j¯Á³o¶¡©Ð¶¡....
if roomvalue($0) == 0
	if class($n) == 10
		if int($n) <= 15
			if rand(20)
				callout 2 echoat $n [1;31m§Aµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b¡A¥²¶·­n¸Ñ°£(disable trap)¡C[m
				callout 2 echoaround $n $n [1;31mµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b....¡C[m
			else
				if rand(40)
					callout 2 echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
				else
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 17
			if rand(40)
				callout 2 echoat $n [1;31m§Aµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b¡A¥²¶·­n¸Ñ°£(disable trap)¡C[m
				callout 2 echoaround $n $n [1;31mµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b....¡C[m
			else
				if rand(60)
					callout 2 echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
				else
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 19
			if rand(60)
				callout 2 echoat $n [1;31m§Aµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b¡A¥²¶·­n¸Ñ°£(disable trap)¡C[m
				callout 2 echoaround $n $n [1;31mµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b....¡C[m
			else
				if rand(80)
					callout 2 echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
				else
					callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
					callout 2 call dragon_trap $n
				endif
			endif
			break
		endif
		if int($n) <= 21
			if rand(80)
				callout 2 echoat $n [1;31m§Aµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b¡A¥²¶·­n¸Ñ°£(disable trap)¡C[m
				callout 2 echoaround $n $n [1;31mµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b....¡C[m
			else
				callout 2 echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
			endif
			break
		endif
		if int($n) <= 23
			callout 2 echoat $n [1;31m§Aµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b¡A¥²¶·­n¸Ñ°£(disable trap)¡C[m
			callout 2 echoaround $n $n [1;31mµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b....¡C[m
		endif
	else
		if rand(5)
			callout 2 echoat $n [1;31m§Aµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b¡A¥²¶·­n¸Ñ°£(disable trap)¡C[m
			callout 2 echoaround $n $n [1;31mµo²{³o¶¡©Ð¶¡¦ü¥G¦³³´¨À¦s¦b....¡C[m
		else
			if rand(20)
				callout 2 echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
			else
				callout 2 echo [1;31mÁV¿|¡I§AÄ²µo¤F³´¨À....¡I[m
				callout 2 call dragon_trap $n
			endif
		endif
	endif
else
	if rand(50)
		echo [1;36m³o¶¡©Ð¶¡ªº³´¨À¤w¸g³Q¸Ñ°£¤F....¡C[m
	else
		echo [1;36m³o¶¡©Ð¶¡¦ü¥G¨S¦³¤°»ò²§¼Ë....¡C[m
	endif
endif
~
>room_trap 251720655 80~
echo [1;31m³Ø°Õ¤@Án¡I§A¦ü¥GÄ²°Ê¤F¾÷Ãö....¡I[m
if roomvalue($0) == 1
	callout 1 echo [1;31m¦ý¬O¦ü¥G¨S¦³¤°»ò¨Æ±¡µo¥Í....¡C[m
else
	call dragon_trap $n
endif
~
>room_defun dragon_trap~
if rand(20)
	echo [1;31m©¿µM¤@°¦Às©¿µM­¸¤F¥X¨Ó¦V§A²r¯P§ðÀ»....[m
	mload 2222
	callout 1 force dragon hunt $r
	break
endif
if rand(25)
	echo [1;31m¤ÑªáªO»P¥|©PªºÀð¾À©¿µM®g¥X¤F´X¤ä­¸Ãð....[m
	if class($n) == 10
		callout 1 echoat $n [1;36m¨­¬°§ÔªÌªº§A¥H¯Â¼ôªº§Þ¥©ÀÉ¦í¤F³o´X¤ä­¸Ãð....¡C[m
		callout 1 echoaround $n [1;36m¨­¬°§ÔªÌªº $n ¥H¯Â¼ôªº§Þ¥©ÀÉ¦í¤F³o´X¤ä­¸Ãð....¡C[m
		callout 1 oload 10208 viclv $n floor
	else
		if rand(50)
			callout 1 echo [1;33m³o´X¤ä­¸Ãð³y¦¨¤F§A¤£¤pªº¶Ë®`....¡C[m
			callout 1 modhmv pc 03.10 25 0
		else
			callout 1 echo [0;36m§A»°ºò¤@°{¡A­¸Ãð±°¨­¦Ó¹L....¡C[m
		endif
		callout 2 oload 10208 viclv $n floor
	endif
	break
endif
if rand(33)
	echo [1;31m¤@¸sÀsÃ~©¿µM¥X²{¦V§A§ð¤F¹L¨Ó....[m
	mload 10253
	mload 10252
	force medium mpgroup self leader 0 1
	mload 10250
	force patrol mpgroup self leader 0 1
	force leader hunt $n
	force leader c fireball $n
else
	echo [1;32m¤ÑªáªO»P¥|©PªºÀð¾À©¿µM¼Q¥X¤F¬r¤ô....[m
	if rand(40)
		callout 1 echo [1;32m³o¨Ç¬r¤ôÅý§A¤¤¬r¤F....¡C[m
		callout 1 add_affect pc poison 0 50 1 -5 4096
		break
	endif
	if rand(45)
		callout 1 echo [1;32m³o¨Ç¬r¤ôÄY­««I»k§Aªº¥Ö½§....¡C[m
		callout 1 modhmv pc 03.10 25 0
	else
		callout 1 echo [1;32m³o¨Ç¬r¤ôÅý§A¤¤¬r¤F....¡C[m
		callout 1 echo [1;32m³o¨Ç¬r¤ôÄY­««I»k§Aªº¥Ö½§....¡C[m
		callout 1 add_affect pc poison 0 50 1 -5 4096
		callout 1 modhmv pc 03.10 25 0
	endif
endif
~
|