>room_defun holytown_iceseal_hunt~
if rand(30)
    if isaffected($r) / prot. from cold
        c 'chill touch' $r 60
    else
        echo [1;37m�방���H���S�A�����j�_�A���A�Ԥ����N�����Y���@�ΡC[0m
        echoat $r [1;36m�B�N���H���᪺�A�������·��F�C[0m
        echoaround $r [1;36m�A�ݨ�[0m $r [1;36m�Q���B�N���H�����������·��F�C[0m
        add_affect $r 'paralyze' 1135 2 39 100 268435456
    endif
else
    if rand(70)
        c 'colour spray' $r 60
    else
        c 'glacier strike' $r 60
    endif
endif
~
|
