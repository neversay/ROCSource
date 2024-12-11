>room_enter 127 35~
if ispc($n)
        if dex($n) > 20
                echoat $n 巨大的枝葉樹籽朝你站的位置落了下來！
                echoat $n 千均一髮之際，你憑著過人的速度與反應閃開了！
                echoaround $n 你看見$N漂亮地閃過了掉下來的巨大枝葉。
        else
                echoat $n 巨大的枝葉樹籽朝你站的位置落了下來！
		if isaffected($n) / stone skin
			echoat $n 你來不及閃開被砸個正著，幸好身上的石甲術讓你沒傷的那麼嚴重。
			echoaround $n 你看見$N被掉下來的巨大枝葉給砸個正著，但看起來只受點輕傷。
			modhmv $n 01.200 200 0
		else
                	echoat $n 你來不及閃開，被砸成重傷！
                	echoaround $n 你看見$N被掉下來的巨大枝葉給砸個正著，真慘......
                	modhmv $n 01.800 1200 0
		endif
        endif
endif
~
>room_defun sealevil_room_5f~
if ispc($r)
        if dex($r) > 20
                echoat $r 巨大的枝葉樹籽朝你站的位置落了下來！
                echoat $r 千均一髮之際，你憑著過人的速度與反應閃開了！
                echoaround $r 你看見$R漂亮地閃過了掉下來的巨大枝葉。
        else
                echoat $r 巨大的枝葉樹籽朝你站的位置落了下來！
		if isaffected($r) / stone skin
			echoat $r 你來不及閃開被砸個正著，幸好身上的石甲術讓你沒傷的那麼嚴重。
			echoaround $r 你看見$R被掉下來的巨大枝葉給砸個正著，但看起來只受點輕傷。
			modhmv $r 01.200 200 0
		else
                	echoat $r 你來不及閃開，被砸成重傷！
                	echoaround $r 你看見$R被掉下來的巨大枝葉給砸個正著，真慘......
                	modhmv $r 01.800 1200 0
		endif
        endif
endif
~
|