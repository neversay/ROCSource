>room_enter 127 100~
if ispc($n)
	echoat $n 你發現這裡可以查閱你的任務紀錄(check quest)。
endif
~
>room_command check quest~
if ispc($n)
	def_rembch $n
	rcall quest_list $n
	echoat $n 你可以鍵入頁數(ex: check 01)來查閱
endif
~
>room_command check 00 3/~
if ispc($n)
	def_rembch $n
	rcall q00 $n
endif
~
>room_command check 01 1/main1~
if ispc($n)
	def_rembch $n
	rcall q01 $n
endif
~
>room_command check 02 1/main2~
if ispc($n)
	def_rembch $n
	rcall q02 $n
endif
~
>room_command check 03 1/west1~
if ispc($n)
	def_rembch $n
	rcall q03 $n
endif
~
>room_command check 04 1/west2~
if ispc($n)
	def_rembch $n
	rcall q04 $n
endif
~
>room_command check 05 1/east1~
if ispc($n)
	def_rembch $n
	rcall q05 $n
endif
~
>room_command check 06 1/east2~
if ispc($n)
	def_rembch $n
	rcall q06 $n
endif
~
>room_command check 07 1/other1~
if ispc($n)
	def_rembch $n
	rcall q07 $n
endif
~
>room_command check 08 1/other2~
if ispc($n)
	def_rembch $n
	rcall q08 $n
endif
~
>room_command check 09 2/main~
if ispc($n)
	def_rembch $n
	rcall q09 $n
endif
~
>room_command check 10 2/naytil~
if ispc($n)
	def_rembch $n
	rcall q10 $n
endif
~
>room_command check 11 2/wilderness~
if ispc($n)
	def_rembch $n
	rcall q11 $n
endif
~
>room_command check 12 2/wilderness2~
if ispc($n)
	def_rembch $n
	rcall q12 $n
endif
~
>room_command check 13 2/mythdale~
if ispc($n)
	def_rembch $n
	rcall q13 $n
endif
~
|
