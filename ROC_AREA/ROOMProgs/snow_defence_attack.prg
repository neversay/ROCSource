>room_enter 127 100~
if rand(20)
        if wis($n) < 15
                echoat $r $n不慎觸動了機關，一支利箭自暗處疾射而出，向你飛來！
                echoaround $r $n不慎觸動了機關，一支利箭自暗處疾射而出，向$r飛去！
                if dex($r) > 20
                        echoat $r 你飛快地朝旁邊閃開，利箭自你身邊掠過，沒射中你。
                        echoaround $r $r漂亮地閃掉了這一箭！
                        if wis($r) < 15
                                echoat $r 但是你在閃避的過程中又不慎踩中了機關！地板噴出了烈焰燒傷了所有人！
                                echoaround $r 然而，$r不小心觸動了另一個機關，火焰忽然噴出，燒傷了你！
                                modhmv pc 01.300 300 0
                        endif
                else
                        choat $r 你來不及閃避！箭射穿了你的肩膀，一陣刺骨劇痛傳來！
                        modhmv $r 01.500 500 0
                        echoaround $r $r被飛箭射中了！
                endif
        endif
endif
~
|
