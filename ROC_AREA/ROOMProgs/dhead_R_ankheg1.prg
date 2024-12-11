>room_enter 15 50~
    if ispc($n)
       if isaffected($n) & 2
          or isaffected($n) & 524288
            echoat $n 你掉入掘地線蟲的陷阱了!!
            echoaround $n 你看見$n掉入掘地線蟲的陷阱了!!
            if rand(25)
                transfer $n 50020
                modhmv $n 03.5 15 11000001
                mload 50000 50020
                break
            endif
            if rand(33)
                transfer $n 50021
                modhmv $n 03.5 15 11000001
                mload 50000 50021
                break
            endif
            if rand(50)
                transfer $n 50022
                modhmv $n 03.5 15 11000001
                mload 50000 50022
                break
            endif
                transfer $n 50023
                modhmv $n 03.5 15 11000001
                mload 50000 50023
        endif
    endif
~
|