>room_enter 127 100~
if roomtimer($0) == 0
        if is_inv_obj($n) == 32112
                if rand(20)
                        echoat $n 你身上的鎖門怪之鑰發出了金黃的光芒！
                        echoaround $n $n 身上的的鎖門怪之鑰發出了金黃的光芒！
                        echo 鑰匙的光芒映照在地板上出現了一道門的形狀！
                        echo 門型的光芒開始外洩，一個門狀洞孔成型！
                        echoat $n 你手中的鑰匙發出高熱讓你握不住！
                        force $n drop key-locked
                        echo 鑰匙快掉到地上前就消失在光芒中了!
                        purge key-locked
                        modrvt v1 = 1
                        modrvt t0 = 3
                else
                        modrvt t0 = 5
                endif
        endif
endif
~
>room_mask down default~
if roomvalue($1) == 1
        if pcnum($) == 1
          echoat $n 當你穿過孔洞之後，孔洞在你身後消失了！
          callout 2 at $n echoaround $n 當 $n 穿過孔洞後，孔洞消失了！
          transfer $n 32265
          modrvt v1 = 0
        else
          echoat $n 你穿過發亮的孔洞往下走去。
          echoaround $n $n 穿過發光的孔洞往下走去！
          transfer $n 32265
        endif
else
        echoat $n [0;33m哎呀，此路不通。[m
endif
~
|
