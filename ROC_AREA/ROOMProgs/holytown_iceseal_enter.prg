>room_enter 127 100~
if ispc($n)
    if rand(60)
        if rand(40)
            if isaffected($n) / prot. from cold
                c 'chill touch' $n 60
            else
                echoat $n [1;36m�B�N���H���᪺�A�������·��F�C[0m
                echoaround $n [1;36m�A�ݨ�[0m $n [1;36m�Q���B�N���H�����������·��F�C[0m
                add_affect $n 'paralyze' 1135 2 39 100 268435456
            endif
        else
            if rand(60)
                c 'colour spray' $n 60
            else
                c 'chill touch' $n 60
            endif
        endif
    else
        echoat $n [1;37m�ﭱ�ӨӪ��H�����A���餣�_��Ÿ�ݵۡA����O�]���ֳt���U�����C[0m
        modhmv $n 23.20 30 00012300
        modhmv $n 03.5 10 00012300
    endif
endif
if roomvalue($n24130v1) != 1
    if pcnum($24125) > 0
    if pcnum($24126) > 0
    if pcnum($24127) > 0
    if pcnum($24128) > 0
    if pcnum($24129) > 0
    if pcnum($24130) > 0
        at 24125 echo [1;36m���ɦ]���L�k�Ө��t���A�}�l���ͼ@�P���n�̡C[0m
        at 24125 echo [1;36m�b�@�P���n�̤��ڡA�H�B�u�æA���a�q��������ΦӥX�C[0m
        at 24125 mload 24044
        at 24125 mload 24044
        at 24126 echo [1;36m���ɦ]���L�k�Ө��t���A�}�l���ͼ@�P���n�̡C[0m
        at 24126 echo [1;36m�b�@�P���n�̤��ڡA�H�B�u�æA���a�q��������ΦӥX�C[0m
        at 24126 mload 24044
        at 24126 mload 24044
        at 24127 echo [1;36m���ɦ]���L�k�Ө��t���A�}�l���ͼ@�P���n�̡C[0m
        at 24127 echo [1;36m�b�@�P���n�̤��ڡA�H�B�u�æA���a�q��������ΦӥX�C[0m
        at 24127 mload 24044
        at 24127 mload 24044
        at 24128 echo [1;36m���ɦ]���L�k�Ө��t���A�}�l���ͼ@�P���n�̡C[0m
        at 24128 echo [1;36m�b�@�P���n�̤��ڡA�H�B�u�æA���a�q��������ΦӥX�C[0m
        at 24128 mload 24044
        at 24128 mload 24044
        at 24129 echo [1;36m���ɦ]���L�k�Ө��t���A�}�l���ͼ@�P���n�̡C[0m
        at 24129 echo [1;36m�b�@�P���n�̤��ڡA�H�B�u�æA���a�q��������ΦӥX�C[0m
        at 24129 mload 24044
        at 24129 mload 24044
        at 24130 echo [1;36m���ɦ]���L�k�Ө��t���A�}�l���ͼ@�P���n�̡C[0m
        at 24130 echo [1;36m�b�@�P���n�̤��ڡA�H�B�u�æA���a�q��������ΦӥX�C[0m
        at 24130 mload 24044
        at 24130 mload 24044
        modrvt 24130 v1 = 1
        callout 1 rcall holytown_iceseal_door $n
    endif
    endif
    endif
    endif
    endif
    endif
endif
if roomvalue($n24130v1) != 1
    rcall holytown_iceseal_mload $n
endif
~
|
