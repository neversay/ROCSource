>room_command open tankard~
    if exitinfo($1) & 4
        echo �F�䪺���w�w�a���}�F
        setdoor 1 flags 113
    endif	
~
>room_time 100~
    if exitinfo($1) !& 4
        echo �F�䪺���w�w�a���W�F
        setdoor 1 flags 119
    endif	
~
|