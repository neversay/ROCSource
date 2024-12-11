>room_command open tankard~
    if exitinfo($1) & 4
        echo 東邊的門緩緩地打開了
        setdoor 1 flags 113
    endif	
~
>room_time 100~
    if exitinfo($1) !& 4
        echo 東邊的門緩緩地關上了
        setdoor 1 flags 119
    endif	
~
|