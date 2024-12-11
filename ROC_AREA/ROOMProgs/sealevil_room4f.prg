>room_enter 127 100~
if ispc($n)
        if isaffected($n) / day of protection
                echoat $n 詛咒的力量被你周圍的神聖防壁擋住了，近不了你的身。
        else
                if rand(33)
                        echoat $n 邪氣化成了厲鬼的外表，穿過了你的身體！
                        echoat $n 你雖然沒受傷，卻多了股難受的感覺。
                        add_affect $n 'curse' 1140 10 24 20 1024
                else
                        if rand(50)
                                echoat $n 你覺得似乎有什麼東西壓制著你的能力。
                                echoat $n 這時，你赫然發現自己已經被不明的闇光籠罩！
                                add_affect $n 'seal magic' 1140 10 40 -30 16777216
                        else
                                echoat $n 強大的怨念波動毫無預警地襲來，直擊你的靈魂！
                                echoat $n 你感到一股暴力慾望在你心中急速增長！
                                add_affect $n 'demon tremor' 1140 10 35 30 536870912
                        endif
                endif
        endif
endif
~
>room_defun sealevil_room_4f~
if ispc($r)
        if isaffected($r) / day of protection
                echoat $r 詛咒的力量被你周圍的神聖防壁擋住了，近不了你的身。
        else
                if rand(33)
                        echoat $r 邪氣化成了厲鬼的外表，穿過了你的身體！
                        echoat $r 你雖然沒受傷，卻多了股難受的感覺。
                        add_affect $r 'curse' 1140 10 24 20 1024
                else
                        if rand(50)
                                echoat $r 你覺得似乎有什麼東西壓制著你的能力。
                                echoat $r 這時，你赫然發現自己已經被不明的闇光籠罩！
                                add_affect $r 'seal magic' 1140 10 40 -30 16777216
                        else
                                echoat $r 強大的怨念波動毫無預警地襲來，直擊你的靈魂！
                                echoat $r 你感到一股暴力慾望在你心中急速增長！
                                add_affect $r 'demon tremor' 1140 10 35 30 536870912
                        endif
                endif
        endif
endif
~
|