>defun_prog dhead_tankard_closedoor~
    mpgoto 50003
    mpecho ���䪺�۪��w�w�a���W�F...
    mpforcer here deldoor 1
~
>speech_prog open tankard~
    mpgoto 50003
    mpecho �F�䪺�۪��w�w�a���}�F...
    mpforcer here makedoor 1 �F��q���~�� tankard 1 0 50001
    mpcall 5 dhead_tankard_closedoor $n
~
>entry_prog 100~
    if inroom(self) != 50003
        mpgoto 50003
    endif
~
|