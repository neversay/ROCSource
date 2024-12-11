>room_defun call_golem~
if roomvalue($0) < 20
	if npcnum($45003) < 2
		echoat $n 地上突然冒出一隻巨石魔怪，向你展開攻擊！
		echoaround $n 地上突然冒出一隻巨石魔怪，向$N展開攻擊！
		mload 45003
		modrvt v0 + 1
		force itrial-golem mpgroup itrial-earth self 1
		force itrial-golem mpkill $n
	endif
endif
~
>room_defun call_earth~
if roomvalue($1) == 0
	modrvt v1 = 1
	mload 45002
endif
~
>room_defun reset_roomvalue~
if roomvalue($0) != 0
	modrvt v0 = 0
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
