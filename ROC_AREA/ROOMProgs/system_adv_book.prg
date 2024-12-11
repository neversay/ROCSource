>room_enter 127 100~
if is_qmark($n) != adv_member_mark
	break
endif
if ispc($n)
	echoat $n 你發現這裡可以查閱你的冒險者手冊(check book)。
endif
~
>room_command check book~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall boss_list $n
	echoat $n 你可以鍵入頁數(ex: check 01 或 check boss01)來查閱
endif
~
>room_command check 01 boss01~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b01 $n
endif
~
>room_command check 02 boss02~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b02 $n
endif
~
>room_command check 03 boss03~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b03 $n
endif
~
>room_command check 04 boss04~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b04 $n
endif
~
>room_command check 05 boss05~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b05 $n
endif
~
>room_command check 06 boss06~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b06 $n
endif
~
>room_command check 07 boss07~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b07 $n
endif
~
>room_command check 08 boss08~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b08 $n
endif
~
>room_command check 09 boss09~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b09 $n
endif
~
>room_command check 10 boss10~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b10 $n
endif
~
>room_command check 11 boss11~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b11 $n
endif
~
>room_command check 12 boss12~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b12 $n
endif
~
>room_command check 13 boss13~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b13 $n
endif
~
>room_command check 14 boss14~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b14 $n
endif
~
>room_command check 15 boss15~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b15 $n
endif
~
>room_command check 16 boss16~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b16 $n
endif
~
>room_command check 17 boss17~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b17 $n
endif
~
>room_command check 18 boss18~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b18 $n
endif
~
>room_command check 19 boss19~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b19 $n
endif
~
>room_command check 20 boss20~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b20 $n
endif
~
>room_command check 21 boss21~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b21 $n
endif
~
>room_command check 22 boss22~
if is_qmark($n) != adv_member_mark
	echoat $n 你還未登記加入冒險者公會，你沒有資格查詢！
	break
endif
if ispc($n)
	def_rembch $n
	rcall b22 $n
endif
~
|