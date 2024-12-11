>room_enter 127 5~
if ispc($n)
        echoat $n 清脆的爆裂聲在你腦海中響起，就像是玻璃破碎的聲音。
        echoat $n 你身上所有的法術不知道什麼時候全消失了！
        del_affect $n all
endif
~
>room_defun sealevil_room_hopeless~
if ispc($r)
        echoat $r 清脆的爆裂聲在你腦海中響起，就像是玻璃破碎的聲音。
        echoat $r 你身上所有的法術不知道什麼時候全消失了！
        del_affect $r all
endif
~
|