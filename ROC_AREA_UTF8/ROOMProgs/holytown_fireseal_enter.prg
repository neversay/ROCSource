>room_enter 127 100~
if ispc($n)
        if isaffected($n) / prot. from fire
                echoat $n [1;36m幸好有著火焰防壁的防護，不然這灼熱的高溫可能會直接要了你的小命。[0m
                modhmv $n 03.5 10 00011401
        else
                echoat $n [1;31m因為沒有火焰防壁的防護，所以才剛走進這裡，你便被四周的高溫給灼傷。[0m
                echoaround $n [1;31m你看到[0m $n [1;31m耐不住四周的高溫，而被嚴重灼傷。[0m
                modhmv $n 03.30 40 00011601
                if rand(60)
                        c 'star fire' $n 40
                else
                        c 'angry fire storm' $n 40
                endif
        endif
endif
echoat $n [1;35m四周灼熱的溫度，加上處於精神緊繃的狀態，讓你不禁汗流浹背，體力大量的流失。[0m
modhmv $n 23.30 40 00011600
modhmv $n 21.100 150 00011600
~
|
