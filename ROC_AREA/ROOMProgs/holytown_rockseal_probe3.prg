>room_command probe south~
if isfight($n)
    echoat $n [1;36m�A���b���۾԰��O�I���Ӫ��ɶ������r�I[0m
else
echoat $n �A�}�l�����۫n�誺�۾��C
    if wis($n) > 23
        callout 1 rcall holytown_rockseal_check_ending3 $n
    else
        callout 1 echoat $n [1;36m�b�A�J�ӱ�������A�٬O����o�n�誺�۾��s�����S�������C[0m
        callout 1 modhmv $n 33.5 10 0
    endif
endif
~
|
