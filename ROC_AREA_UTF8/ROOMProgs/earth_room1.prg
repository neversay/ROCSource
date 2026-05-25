>room_enter 127 100~
if ispc($n)
	if roomvalue($0) == 0
		rcall call_golem $n
	endif
endif
~
>room_defun call_golem~
echo 大地震動，巨石魔怪從沉睡中醒了過來！
modrvt v0 = 1
oload 45003
force itrial-golem mpkill $n
if rand(10)
	oload 45003
	force itrial-golem mpkill $n
endif
if rand(50)
	oload 45003
	force itrial-golem mpkill $n
endif
~
>room_defun reset_roomvalue~
if rnplayer($) == 0
	if roomvalue($0) != 0
		modrvt v0 = 0
	endif
endif
~
>room_defun earth_room1~
echo 大地突然猛烈地震動起來！
if ispc($r)
	rcall start_earth $r
endif
~
>room_defun start_earth~
if rand(60)
	echoat $r 強烈的地震形使地形產生變動，你站不穩住腳，丟進大地的裂縫之中！
	echoaround $r 強烈的地震形使地形產生變動，$R站不穩住腳，丟進大地的裂縫之中！
	rcall earth_damage $r
	if rand(40)
		echoat $r 回過神來，你發現自己迷失了方向！！！！！！
		echoaround $r 回過神來，你發現$R已經不知去向！！！！！！
		rcall start_transfer $r
	endif
endif
~
>room_defun start_transfer~
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
~
>room_defun earth_damage~
if rand(33)
	echoat $r 強烈的地震讓你 [1;32m* 疼痛不堪 *[m!(151-190)
	echoaround $r 強烈的地震讓$R [1;32m* 疼痛不堪 *[m!
	modhmv $r 01.151 190 0
	break
endif
	if rand(66)
		echoat $r 強烈的地震讓你 [1;32m** 疼痛不堪 **[m!(191-230)
		echoaround $r 強烈的地震讓$R [1;32m** 疼痛不堪 **[m!
		modhmv $r 01.191 230 0
	else
		echoat $r 強烈的地震打得你 [1;32m** 尖聲哀叫 **[m!(231-280)
		echoaround $r 強烈的地震打得$R [1;32m** 尖聲哀叫 **[m!
		modhmv $r 01.281 330 0
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
