>room_defun holytown_fireseal_hunt~
if rand(60)
        if isaffected($r) / prot. from fire
                echo [1;36m�|�P���۾���M«�X�@�D���]�A�襩���b[0m $r [1;36m�����K�����W�C[0m
                modhmv $r 01.50 80 00011401
        else
                if rand(30)
                        echo [1;36m�|�P���۾���M«�X�@�D���]�A�襩���b[0m $r [1;36m�����W�C[0m
                        echoat $r [1;31m�]���S�����K�������O�@�A�ҥH�A�˪������C[0m
                        modhmv $r 03.40 50 00011601
                        echoaround $r [1;31m�A�ݨ�[0m $r [1;31m���G�Q���˪������C�ߤ����K�y���ä��O�N��A�C[0m
                else
                        if rand(60)
                                c flamestrike $r 40
                        else
                                mload 24046
                        endif
                endif
        endif
else
        if rmobcount(24043) < 4
                echo [1;36m�|�P���۾���M«�X�@�D���]�A���[��N�ܦ��F�@�έ����µۧA���䭸�L�ӡC[0m
                mload 24043
        else
                if rand(40)
                        mload 24046
                else
                        c fireball $r 40
                endif
        endif
endif
echo [1;35m�|�P�`�����ūסA�[�W�B��믫���^�����A�A���A���T���y�H�I�A��O�j�q���y���C[0m
modhmv pc 23.20 30 00011600
~
|
