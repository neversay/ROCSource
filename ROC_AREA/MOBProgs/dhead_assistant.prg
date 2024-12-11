>defun_prog dhead-assistant-cure1~
    mpadd_qmark self dhead_assistant_tickcount 3 0 0 0 1 0
    mpadd_qmark self dhead_assistant_turncount -1 0 0 0 1 0
    mpdef_rembch $n
    mpcallout 1 say ���ߧA��o�Ĥ@���ӧQ�C
    mpcallout 2 say �b�ĤG�����ɶ}�l�e�A�A���|�Ӥp�ɪ��ɶ��i�H�𮧡C
    mpcallout 3 say �ڷ|���A�I�@�Ǧ^�_�k�N
~
>defun_prog dhead-assistant-cure2~
    mpadd_qmark self dhead_assistant_tickcount 3 0 0 0 1 0
    mpmod_qmark self dhead_assistant_turncount v0 = 1
    mpcallout 1 say ���ߧA��o�Ĥ@���ӧQ�C
    mpcallout 2 say �b�ĤG�����ɶ}�l�e�A�A���|�Ӥp�ɪ��ɶ��i�H�𮧡C
    mpcallout 3 say �ڷ|���A�I�@�Ǧ^�_�k�N
~
>defun_prog dhead-assistant-cong~
    mpmod_qmark self dhead_assistant_turncount v0 = 2
    say ���ߧA��o�̫᪺�ӧQ�C
    mpcall dhead-assistant-reward $n
~
>defun_prog dhead_assistant-questcomplete~
    mpexp $n 5000
    mpdel_qmark $n dhead_instructor_contestquest
    mpdel_qmark $n dhead_instructor_tickcount
    if is_qmark($n) != dhead_allquest
        mpadd_qmark $n dhead_allquest -1 0 0 0 0 1
    endif
    mpmod_qmark $n dhead_allquest v0 | 8
    yell �ګŧG $n �q�L�h�i�H�Ԥh�շҡI
    mplog $n pass the lizardman quest
    mpgoto corecle-dhead mppurge corecle-dhead
    mpat 50125 mpmload 50130
    mpgoto instructor-dhead mppurge instructor-dhead
    mpat 50126 mpmload 50131
    mpgoto $b
    mpechoat $b ���ۡA�A�Q�e�X�F�v�޳��C
    mptransfer $b 50126
    mppurge self
~
>defun_prog dhead_assistant-questreward~
    if is_qmark($n) & dhead_instructor_contestquest 100002
        mpmod_qmark $n dhead_instructor_contestquest v2 + 1
    else
        if is_qmark($n) & dhead_instructor_contestquest 100004
            mpmod_qmark $n dhead_instructor_contestquest v1 + 3
        else
            if is_qmark($n) & dhead_instructor_contestquest 100008
                mpmod_qmark $n dhead_instructor_contestquest v1 + 8
            endif
        endif
    endif
    if is_qmark($n) == dhead_instructor_contestquest 200004
        mpcallout 1 say ���M�A���@�Ԥ覡�ä��ŦX�Ԥh��ź�ơA
        mpcallout 2 say �������B�e���ۧڪ���h�A���¬O�O�H�ܨت��C
        mpcallout 3 say ���D�ӧQ���ܤ�q...�o�O�A���o�����y�C
        mpcallout 4 mpoload 50100 37 $n
        mpcallout 4 mpechoat $n $I�N�@��t��⪺�ܪӥ浹�F�A�C
        mpcallout 4 mpcall dhead_assistant-questcomplete $n
    else
        if is_qmark($n) > dhead_instructor_contestquest 100012
            mpcallout 1 say �M���C�v���M���U�A�|���l�O�U�Ψ쩼�����ͩR�C
            mpcallout 2 say �ץH���O���ߩ��A��⪺�x�ܡA��o�̫᪺�ӧQ�C
            mpcallout 3 say �ڡ㰪�Q���Ԥh�A�o�O�A���o�����y�C
            mpcallout 4 mpoload 50101 37 $n
            mpcallout 4 mpechoat $n $I�N�@��G�զ⪺�Ҵߥ浹�F�A�C
            mpcallout 4 mpcall dhead_assistant-questcomplete $n
        else
            if is_qmark($n) < dhead_instructor_tickprog 100050
                mpcallout 1 say �A���i��P�Z�޵L�P�ۤ�A
                mpcallout 2 say �C�C�@�X��A�ӧQ�������N���t�a���A�T�_�C
                mpcallout 3 say �ڡ�i�r���Ԥh�A�o�O�A���o�����y�C
                mpcallout 4 mpoload 50102 37 $n
                mpcallout 4 mpechoat $n $I�N�@���`���⪺��ޥ浹�F�A�C
                mpcallout 4 mpcall dhead_assistant-questcomplete $n
            else
                if is_qmark($n) > dhead_instructor_contestquest 200000
                    mpcallout 1 say ���M�A���g���X���H�Ԥh�믫���|�ʡA
                    mpcallout 2 say ���������լD�Ԫ��i�𤴵M�Q���i���C
                    mpcallout 3 say �L�צp��A�o�O�A���o�����y�C
                    mpcallout 4 mpoload 50103 37 $n
                    mpcallout 4 mpechoat $n $I�N�浹�F�A�C
                    mpcallout 4 mpcall dhead_assistant-questcomplete $n
                else
                    mpcallout 1 say �A����{�۷�a�ǥX�C
                    mpcallout 2 say �ר䨺�����լD�Ԫ��i�𤴵M�Q���i���C
                    mpcallout 3 say �o�O�A���o�����y�C
                    mpcallout 4 mpoload 50104 37 $n
                    mpcallout 4 mpechoat $n $I�N�浹�F�A�C
                    mpcallout 4 mpcall dhead_assistant-questcomplete $n
                endif
            endif
        endif
    endif
~
>defun_prog dhead_assistant_randprog~
    if is_qmark(self) == dhead_assistant_tickcount
        mpadd_qmark self dhead_assistant_casting -1 0 0 0 1 0
        if rand(20)
            c 'heal' $r
        else
            if rand(25)
                c 'refresh' $r
            else
                if rand(33)
                    c bless $r
                else
                    if rand(50)
                        c 'cure critical' $r
                    else
                        c 'giant strength' $r
                    endif
                endif
            endif
        endif
        mpdel_qmark self dhead_assistant_casting
    endif
~
>defun_prog dhead_assistant_tickprog~
    if pcnum($) > 0
        if is_qmark(self) == dhead_assistant_tickcount
            break
        else
            if rembch($b)
                if being($b)
                    if is_qmark(self) == dhead_assistant_tickout
                        mpdel_qmark self dhead_assistant_tickout
                        mpmod_qmark self dhead_assistant_turncount v0 + 1
                        mpechoat $b �Ať��԰��Y�N�}�l���a�n�C
                        mpechoat $b ���۴N�Q�a���v�޳��W�F...
                        mptransfer $b 50128
                        mpgoto 50129
                        if is_qmark(self) == dhead_assistant_turncount 100001
                            mpmload 50141
                        else
                            mpmload 50142
                        endif
                        mpgoto 50127
                    else
                        mpadd_qmark self dhead_assistant_tickout -1 0 0 0 1 0
                        wake $b
                        say �U�@���԰��Y�N�}�l�A�а��n�ǳơC
                    endif
                endif
            endif
        endif
    endif
~
|