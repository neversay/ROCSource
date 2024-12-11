
>room_enter 127 100~
if ispc($n)
	if roomvalue($0) == 0
		rcall call_golem $n
	endif
endif
~
>room_defun call_golem~
echo ¤j¦a¾_°Ê¡A¥¨¥ÛÅ]©Ç±q¨IºÎ¤¤¿ô¤F¹L¨Ó¡I
modrvt v0 = 1
oload 45003
force itrial-golem mpkill $r
if rand(5)
	oload 45003
	force itrial-golem mpkill $r
endif
if rand(10)
	oload 45003
	force itrial-golem mpkill $r
endif
if rand(40)
	oload 45003
	force itrial-golem mpkill $r
endif
if rand(70)
	oload 45003
	force itrial-golem mpkill $r
endif
~
>room_defun reset_roomvalue~
if rnplayer($) == 0
	if roomvalue($0) != 0
		modrvt v0 = 0
	endif
endif
~
>room_defun earth_room2~
echo ¤j¦a¬ðµM²r¯P¦a¾_°Ê°_¨Ó¡I
if ispc($r)
	rcall start_earth $r
endif
~
>room_defun start_earth~
if rand(50)
	echoat $n ±j¯Pªº¦a¾_§Î¨Ï¦a§Î²£¥ÍÅÜ°Ê¡A§A¯¸¤£Ã­¦í¸}¡A¥á¶i¤j¦aªºµõÁ_¤§¤¤¡I
	echoaround $n ±j¯Pªº¦a¾_§Î¨Ï¦a§Î²£¥ÍÅÜ°Ê¡A$R¯¸¤£Ã­¦í¸}¡A¥á¶i¤j¦aªºµõÁ_¤§¤¤¡I
	rcall earth_damage $r
	if rand(60)
		echoat $r ¦^¹L¯«¨Ó¡A§Aµo²{¦Û¤v°g¥¢¤F¤è¦V¡I¡I¡I¡I¡I¡I
		echoaround $r ¦^¹L¯«¨Ó¡A§Aµo²{$R¤w¸g¤£ª¾¥h¦V¡I¡I¡I¡I¡I¡I
		rcall start_transfer $r
	endif
endif
~
>room_defun start_transfer~
if rand(40)
	if rand(3)
		transfer $r 45151
		break
	endif
	if rand(6)
		transfer $r 45140
		break
	endif
	if rand(9)
		transfer $r 45149
		break
	endif
	if rand(12)
		transfer $r 45133
		break
	endif
	if rand(15)
		transfer $r 45141
		break
	endif
	if rand(18)
		transfer $r 45126
		break
	endif
	if rand(21)
		transfer $r 45134
		break
	endif
	if rand(24)
		transfer $r 45139
		break
	endif
	if rand(27)
		transfer $r 45127
		break
	endif
	if rand(30)
		transfer $r 45142
		break
	endif
	if rand(33)
		transfer $r 45150
		break
	endif
	if rand(36)
		transfer $r 45135
		break
	endif
	if rand(39)
		transfer $r 45143
		break
	endif
	if rand(42)
		transfer $r 45152
		break
	endif
	if rand(45)
		transfer $r 45128
		break
	endif
	if rand(48)
		transfer $r 45153
		break
	endif
	if rand(51)
		transfer $r 45129
		break
	endif
	if rand(54)
		transfer $r 45144
		break
	endif
	if rand(57)
		transfer $r 45130
		break
	endif
	if rand(60)
		transfer $r 45154
		break
	endif
	if rand(63)
		transfer $r 45136
		break
	endif
	if rand(66)
		transfer $r 45146
		break
	endif
	if rand(69)
		transfer $r 45137
		break
	endif
	if rand(72)
		transfer $r 45131
		break
	endif
	if rand(75)
		transfer $r 45155
		break
	endif
	if rand(78)
		transfer $r 45138
		break
	endif
	if rand(81)
		transfer $r 45147
		break
	endif
	if rand(84)
		transfer $r 45132
		break
	endif
	if rand(87)
		transfer $r 45148
		break
	endif
	if rand(90)
		transfer $r 45156
		break
	endif
	if rand(93)
		transfer $r 45125
		break
	endif
	if rand(96)
		transfer $r 45157
	else
		transfer $r 45145
	endif
else
	if rand(3)
		transfer $r 45185
		break
	endif
	if rand(6)
		transfer $r 45170
		break
	endif
	if rand(9)
		transfer $r 45194
		break
	endif
	if rand(12)
		transfer $r 45158
		break
	endif
	if rand(15)
		transfer $r 45184
		break
	endif
	if rand(18)
		transfer $r 45169
		break
	endif
	if rand(21)
		transfer $r 45178
		break
	endif
	if rand(24)
		transfer $r 45159
		break
	endif
	if rand(27)
		transfer $r 45187
		break
	endif
	if rand(30)
		transfer $r 45171
		break
	endif
	if rand(33)
		transfer $r 45168
		break
	endif
	if rand(36)
		transfer $r 45183
		break
	endif
	if rand(39)
		transfer $r 45160
		break
	endif
	if rand(42)
		transfer $r 45188
		break
	endif
	if rand(45)
		transfer $r 45193
		break
	endif
	if rand(48)
		transfer $r 45167
		break
	endif
	if rand(51)
		transfer $r 45177
		break
	endif
	if rand(54)
		transfer $r 45182
		break
	endif
	if rand(57)
		transfer $r 45189
		break
	endif
	if rand(60)
		transfer $r 45161
		break
	endif
	if rand(63)
		transfer $r 45172
		break
	endif
	if rand(66)
		transfer $r 45166
		break
	endif
	if rand(69)
		transfer $r 45179
		break
	endif
	if rand(72)
		transfer $r 45162
		break
	endif
	if rand(75)
		transfer $r 45190
		break
	endif
	if rand(78)
		transfer $r 45173
		break
	endif
	if rand(81)
		transfer $r 45165
		break
	endif
	if rand(84)
		transfer $r 45180
		break
	endif
		if rand(87)
		transfer $r 45191
		break
	endif
	if rand(90)
		transfer $r 45163
		break
	endif
	if rand(92)
		transfer $r 45192
		break
	endif
	if rand(94)
		transfer $r 45174
		break
	endif
	if rand(96)
		transfer $r 45181
		break
	endif
	if rand(98)
		transfer $r 45175
	else
		transfer $r 45164
	endif
endif
~
>room_defun earth_damage~
if rand(33)
	echoat $r ±j¯Pªº¦a¾_Åý§A [1;32m** ¯kµh¤£³ô **[m!(191-230)
	echoaround $r ±j¯Pªº¦a¾_Åý$R [1;32m** ¯kµh¤£³ô **[m!
	modhmv $r 01.191 230 0
endif
	if rand(66)
		echoat $r ±j¯Pªº¦a¾_¥´±o§A [1;32m** ¦yÁn«s¥s **[m!(231-280)
		echoaround $r ±j¯Pªº¦a¾_¥´±o$R [1;32m** ¦yÁn«s¥s **[m!
		modhmv $r 01.281 330 0
	else
		echoat $r ±j¯Pªº¦a¾_¥´±o§A [1;32m*** ÂA¦åª½¬y ***[m.(331-380)
		echoaround $r ±j¯Pªº¦a¾_¥´±o$R [1;32m*** ÂA¦åª½¬y ***[m.(331-380)
		modhmv pc 01.331 380 0
	endif
endif
~
>room_mask cast g~
~
>room_mask cast ga~
~
>room_mask cast gat~
~
>room_mask cast gate~
~
>room_mask cast c~
~
>room_mask cast cl~
~
>room_mask cast cla~
~
>room_mask cast clay~
~
>room_mask cast clay g~
~
>room_mask cast clay go~
~
>room_mask cast clay gol~
~
>room_mask cast clay gole~
~
>room_mask cast clay golem~
~
>room_mask cast w~
~
>room_mask cast wa~
~
>room_mask cast wat~
~
>room_mask cast wate~
~
>room_mask cast water~
~
>room_mask cast water s~
~
>room_mask cast water sp~
~
>room_mask cast water spi~
~
>room_mask cast water spir~
~
>room_mask cast water spiris~
~
>room_mask cast water spirit~
~
>room_mask cast fire e~
~
>room_mask cast fire el~
~
>room_mask cast fire ele~
~
>room_mask cast fire elem~
~
>room_mask cast fire eleme~
~
>room_mask cast fire elemen~
~
>room_mask cast fire element~
~
>room_mask cast fire elementa~
~
>room_mask cast fire elemental~
~
>room_mask h~
~
>room_mask hs~
~
>room_mask hsu~
~
>room_mask hsum~
~
>room_mask hsumm~
~
>room_mask hsummo~
~
>room_mask hsummon~
~
|
