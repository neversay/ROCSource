>room_enter 127 100~
if ispc($n)
        if isaffected($n) / day of protection
                echoat $n �A�G���O�q�Q�A�P�򪺯��t�����צ�F�A�񤣤F�A�����C
        else
                if rand(33)
                        echoat $n ����Ʀ��F�F�����~��A��L�F�A������I
                        echoat $n �A���M�S���ˡA�o�h�F���������Pı�C
                        add_affect $n 'curse' 1140 10 24 20 1024
                else
                        if rand(50)
                                echoat $n �Aı�o���G������F������ۧA����O�C
                                echoat $n �o�ɡA�A���M�o�{�ۤv�w�g�Q���������Ţ�n�I
                                add_affect $n 'seal magic' 1140 10 40 -30 16777216
                        else
                                echoat $n �j�j������i�ʲ@�L�wĵ�aŧ�ӡA�����A���F��I
                                echoat $n �A�P��@�ѼɤO����b�A�ߤ���t�W���I
                                add_affect $n 'demon tremor' 1140 10 35 30 536870912
                        endif
                endif
        endif
endif
~
>room_defun sealevil_room_4f~
if ispc($r)
        if isaffected($r) / day of protection
                echoat $r �A�G���O�q�Q�A�P�򪺯��t�����צ�F�A�񤣤F�A�����C
        else
                if rand(33)
                        echoat $r ����Ʀ��F�F�����~��A��L�F�A������I
                        echoat $r �A���M�S���ˡA�o�h�F���������Pı�C
                        add_affect $r 'curse' 1140 10 24 20 1024
                else
                        if rand(50)
                                echoat $r �Aı�o���G������F������ۧA����O�C
                                echoat $r �o�ɡA�A���M�o�{�ۤv�w�g�Q���������Ţ�n�I
                                add_affect $r 'seal magic' 1140 10 40 -30 16777216
                        else
                                echoat $r �j�j������i�ʲ@�L�wĵ�aŧ�ӡA�����A���F��I
                                echoat $r �A�P��@�ѼɤO����b�A�ߤ���t�W���I
                                add_affect $r 'demon tremor' 1140 10 35 30 536870912
                        endif
                endif
        endif
endif
~
|