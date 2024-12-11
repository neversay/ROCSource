>room_rand 30~
if ispc($r)
	if isaffected($r) & 524288
		echoat $r 強勁的谷風將你吹往不知名的地方....
		echoaround $r $r 被強勁的谷風給吹走了....
		rpteleport $r 5700 5706 5720
	endif
endif
~
|