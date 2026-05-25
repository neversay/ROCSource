>room_enter 127 100~
if roomvalue($0) == 0
	break
endif
if roomvalue($0) <= 3
	echoat $n [1;36m這裡呈現輕微損傷。[0m
	break
endif
if roomvalue($0) <= 6
	echoat $n [1;32m這裡呈現嚴重損傷，多處房屋起火。[0m
	break
endif
if roomvalue($0) <= 9
	echoat $n [1;33m這裡呈現非常嚴重的損傷，四周陷入一片火海。[0m
	break
else
	echoat $n [1;31m這裡呈現毀滅狀態，窒息的高溫遍佈四周。[0m
	break
endif
~
|