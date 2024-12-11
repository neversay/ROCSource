>room_command pray default~
echoat $n 向你的神祈求保佑吧～
~
>room_command pray pluto~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100001
	modrvt v0 | 1
	stun $n 30
	echoat $n 你向 闇之冥王星普魯多Pluto 大人祈求祝福～～
	echoaround $n $N向 闇之冥王星普魯多Pluto 大人祈求祝福～～
	callout 1 echoat $n 一陣柔和的白光包圍著你....瞬間你已經回到村子裡了！
	callout 1 echoaround $n $N全身發出亮光....消失了！
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n 闇之冥王星普魯多Pluto 不願幫助你....
endif
~
>room_command pray amenda~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100002
	modrvt v0 | 1
	stun $n 30
	echoat $n 你向 狗不理。阿。曼達Amenda 大人祈求祝福～～
	echoaround $n $N向 狗不理。阿。曼達Amenda 大人祈求祝福～～
	callout 1 echoat $n 一陣柔和的白光包圍著你....瞬間你已經回到村子裡了！
	callout 1 echoaround $n $N全身發出亮光....消失了！
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n 狗不理。阿。曼達Amenda 不願幫助你....
endif
~
>room_command pray zcecil~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100003
	modrvt v0 | 1
	stun $n 30
	echoat $n 你向 影子Zcecil 大人祈求祝福～～
	echoaround $n $N向 影子Zcecil 大人祈求祝福～～
	callout 1 echoat $n 一陣柔和的白光包圍著你....瞬間你已經回到村子裡了！
	callout 1 echoaround $n $N全身發出亮光....消失了！
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n 影子Zcecil 不願幫助你....
endif
~
>room_command pray coconet~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100004
	modrvt v0 | 1
	stun $n 30
	echoat $n 你向 椰子神Coconet 大人祈求祝福～～
	echoaround $n $N向 椰子神Coconet 大人祈求祝福～～
	callout 1 echoat $n 一陣柔和的白光包圍著你....瞬間你已經回到村子裡了！
	callout 1 echoaround $n $N全身發出亮光....消失了！
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n 椰子神Coconet 不願幫助你....
endif
~
>room_command pray blaz~
if roomvalue($0) & 1
	break
endif
if is_qmark($n) == labyrinth_quest 100005
	modrvt v0 | 1
	stun $n 30
	echoat $n 你向 大魔王布雷茲Blaz 大人祈求祝福～～
	echoaround $n $N向 大魔王布雷茲Blaz 大人祈求祝福～～
	callout 1 echoat $n 一陣柔和的白光包圍著你....瞬間你已經回到村子裡了！
	callout 1 echoaround $n $N全身發出亮光....消失了！
	callout 2 transfer $n 38216
	callout 2 modrvt v0 r 1
else
	echoat $n 你根本不屑對 大魔王布雷茲Blaz 祈禱....
endif
~
|