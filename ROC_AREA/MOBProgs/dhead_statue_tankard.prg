>defun_prog dhead_tankard_closedoor~
    mpgoto 50003
    mpecho 西邊的石門緩緩地關上了...
    mpforcer here deldoor 1
~
>speech_prog open tankard~
    mpgoto 50003
    mpecho 東邊的石門緩緩地打開了...
    mpforcer here makedoor 1 東邊通往外面 tankard 1 0 50001
    mpcall 5 dhead_tankard_closedoor $n
~
>entry_prog 100~
    if inroom(self) != 50003
        mpgoto 50003
    endif
~
|