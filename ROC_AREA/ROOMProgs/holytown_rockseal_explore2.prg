>room_command explore east~
if isfight($n)
    echoat $n [1;36m�A���b���۾԰��O�I���Ӫ��ɶ������r�I[0m
else
echoat $n �A�}�l�����۪F�誺�۾��C
    if wis($n) > 23
        callout 1 rcall holytown_rockseal_check_ending2 $n
    else
        callout 1 echoat $n [1;36m�b�A�J�ӱ�������A�٬O����o�F�誺�۾��s�����S�������C[0m
        callout 1 modhmv $n 33.5 10 0
    endif
endif
~
|
