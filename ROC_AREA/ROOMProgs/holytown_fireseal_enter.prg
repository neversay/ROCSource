>room_enter 127 100~
if ispc($n)
        if isaffected($n) / prot. from fire
                echoat $n [1;36m���n���ۤ��K���������@�A���M�o�`�������ťi��|�����n�F�A���p�R�C[0m
                modhmv $n 03.5 10 00011401
        else
                echoat $n [1;31m�]���S�����K���������@�A�ҥH�~�訫�i�o�̡A�A�K�Q�|�P�����ŵ��`�ˡC[0m
                echoaround $n [1;31m�A�ݨ�[0m $n [1;31m�@����|�P�����šA�ӳQ�Y���`�ˡC[0m
                modhmv $n 03.30 40 00011601
                if rand(60)
                        c 'star fire' $n 40
                else
                        c 'angry fire storm' $n 40
                endif
        endif
endif
echoat $n [1;35m�|�P�`�����ūסA�[�W�B��믫���^�����A�A���A���T���y�H�I�A��O�j�q���y���C[0m
modhmv $n 23.30 40 00011600
modhmv $n 21.100 150 00011600
~
|
