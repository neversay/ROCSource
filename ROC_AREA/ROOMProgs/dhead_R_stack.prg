>room_defun dhead_R_stack_climbsuccess~
       echoat $n �A�յ��k�V�o�︨��...
       echoaround $n $n �յ��k�V�o�︨��...
       callout  3 at $n echoat   $n ���M����M�I�A���A�٬O����F!!
       callout  4 at $n transfer $n 50055
       callout  4 at $n echoat   $n �A���\�a½�V���۰�A��W�s�D���t�@���C
~
>room_defun dhead_R_stack_climbfail~
       echoat $n �A�յ۪��V�L�o�︨��...
       echoaround $n $n �յ��k�V�o�︨��...
       callout 3 at $n echoat $n �z�I���۬�M�u�ʡA�A�L�U�h�F!!
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