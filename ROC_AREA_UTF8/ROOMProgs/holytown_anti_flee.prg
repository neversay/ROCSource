>room_mask flee default~
if isfight($n)
        echoat $n [1;31m在混戰中驚慌失措而想逃跑的你，被身旁竄出的火苗燒個正著。[0m
        echoaround $n $n[1;31m在混戰中驚慌失措的想要逃跑，卻不幸被身旁竄出的火苗燒個正著。[0m
        modhmv $n 03.20 40 00010401
else
        echoat $n 你熱昏頭了嗎？你並不在戰鬥中啊！
endif
~
|

