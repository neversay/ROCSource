>room_defun dhead_R_stack_climbsuccess~
       echoat $n 你試著攀越這堆落石...
       echoaround $n $n 試著攀越這堆落石...
       callout  3 at $n echoat   $n 雖然很驚危險，但你還是做到了!!
       callout  4 at $n transfer $n 50055
       callout  4 at $n echoat   $n 你成功地翻越落石堆，踏上山道的另一側。
~
>room_defun dhead_R_stack_climbfail~
       echoat $n 你試著爬越過這堆落石...
       echoaround $n $n 試著攀越這堆落石...
       callout 3 at $n echoat $n 哇！落石突然滾動，你摔下去了!!
       callout 3 at $n transfer $n 50052       
~
>room_command climb p rock stacks~
    if dex($n) == 25
        if rand(95)
            rcall dhead_R_stack_climbsuccess $n 100
        else     
            rcall dhead_R_stack_climbfail $n 100
        endif    
        break
    endif
    if dex($n) == 24
        if rand(90)
           rcall dhead_R_stack_climbsuccess $n 100
        else     
           rcall dhead_R_stack_climbfail $n 100
        endif    
    	break
    endif
    if dex($n) == 23
        if rand(85)
           rcall dhead_R_stack_climbsuccess $n 100
        else     
           rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 22
    	if rand(80)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 21
    	if rand(75)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
    	endif
    	break
    endif   
    if dex($n) == 20
    	if rand(70)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 19
    	if rand(65)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 18
    	if rand(60)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 17
    	if rand(55)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 16
    	if rand(50)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 15
    	if rand(45)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 14
    	if rand(40)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 13
    	if rand(35)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 12
    	if rand(30)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 11
    	if rand(25)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 10
    	if rand(20)
            rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 9
    	if rand(15)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    if dex($n) == 8
    	if rand(10)
            rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
        break
    endif
    	if rand(5)
    	    rcall dhead_R_stack_climbsuccess $n 100
    	else     
            rcall dhead_R_stack_climbfail $n 100
        endif
~
|