>room_enter 127 100~
if rand(20)
        if wis($n) < 15
                echoat $r $n���VĲ�ʤF�����A�@��Q�b�۷t�B�e�g�ӥX�A�V�A���ӡI
                echoaround $r $n���VĲ�ʤF�����A�@��Q�b�۷t�B�e�g�ӥX�A�V$r���h�I
                if dex($r) > 20
                        echoat $r �A���֦a�®���{�}�A�Q�b�ۧA���䱰�L�A�S�g���A�C
                        echoaround $r $r�}�G�a�{���F�o�@�b�I
                        if wis($r) < 15
                                echoat $r ���O�A�b�{�ת��L�{���S���V�򤤤F�����I�a�O�Q�X�F�P�K�N�ˤF�Ҧ��H�I
                                echoaround $r �M�ӡA$r���p��Ĳ�ʤF�t�@�Ӿ����A���K���M�Q�X�A�N�ˤF�A�I
                                modhmv pc 01.300 300 0
                        endif
                else
                        choat $r �A�Ӥ��ΰ{�סI�b�g��F�A���ӻH�A�@�}�방�@�h�ǨӡI
                        modhmv $r 01.500 500 0
                        echoaround $r $r�Q���b�g���F�I
                endif
        endif
endif
~
|
