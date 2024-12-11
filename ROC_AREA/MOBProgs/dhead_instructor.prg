>defun_prog dhead_instructor_quest1~
    mpadd_qmark $n dhead_instructor_contestquest -1 0 0 0 0 1
    mpadd_qmark self dhead_instructor_bewaiting 24 0 0 0 1 0
    mpgoto corecle
    mppurge corecle
    mpmload 50130
    mpgoto 50126
~
>defun_prog dhead_instructor_quest2~
    mpmod_qmark $n dhead_instructor_contestquest v0 r 32
    mpmod_qmark $n dhead_instructor_contestquest v0 | 64
    mpadd_qmark $n dhead_instructor_tickcount -1 0 0 0 1 0
    mpdef_rembch $n
    say �[�o�I�A�n�۬����a�I
    mpat 50127 mppurge
    mpat 50128 mppurge
    mpat 50129 mppurge
    mptransfer $n 50128
    mpat $n mpechoat $n �A�Q�a�i�F�v�޳�...
    mpat $n mpechoat $n �|�P��_�[���̿��Ī���ķ�P�w�I�n�C
    mpat $n mpforce $n look
    mpat 50127 mpmload 50132
    mpat 50129 mpmload 50140
~
>defun_prog dhead_instructor_tickprog~
    if rembch($b)
        if being($b)
            if is_qmark($b) dhead_instructor_tickcount
                mpat $b mpmod_qmark $b dhead_instructor_tickcount v0 + 1
            else
                mpgoto 50126
                mpmload 50131
                mppurge self
            endif
        endif
    endif
~
>greet_prog 100~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        if is_qmark($n) & dhead_instructor_contestquest 100016
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpmod_qmark $n dhead_instructor_contestquest v0 r 16
            mpmod_qmark $n dhead_instructor_contestquest v0 | 32            
            mpcallout 1  clap $n
            mpcallout 3  say �ݨӧA�w�g���o�غ��J���{�i�F�C
            mpcallout 4  say ���O���O�����o�Ӧ��C
            mpcallout 5  say ���U�Ӫ��շҡA���{�����u�|��j�I
            mpcallout 6  hmm 
            mpcallout 8  say ����A���ܤ@�U�շҪ����e�C
            mpcallout 9  say �A�����e���v�޳��A���ѤT��j�O�����C
            mpcallout 11 say ���M�u�D�ӭt�A���E�P���԰������|����@�覺�`����C
            mpcallout 13 say �p�G�Aı�o�w�g���n�ǳ�(ready)�A��ڻ��@�n�N�i�H�}�l�F�C
            mpcallout 14 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) & dhead_instructor_contestquest 100064
                mpechoat $n �A���e�D�ԥ��ѤF�I
                mpechoat $n ��$I���@�n(ready)�N�i�H���ӡC
                mpmod_qmark $n dhead_instructor_contestquest v0 | 32
                mpmod_qmark $n dhead_instructor_contestquest v1 = 0
                mpmod_qmark $n dhead_instructor_contestquest v2 = 0
            endif
        endif
    endif
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        if race($n) == 1024
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say ����...�~���H...
            mpcallout 2 say �A�O�Ӹ߰ݾԤh�t�շ�(contest)���ƶ�? 
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        else
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 hmm $n
            mpcallout 2 say �p�G�S���O���ơA�����}�o�̧a�C
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog contest~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        mpcallout 1 say �y...�y...
        mpcallout 2 say �ѥ[�շ�(contest)�D�԰���A�O�ڭ̻h�i�H���̰��aģ�C
        mpcallout 3 say �p�G��q�L�սm�A�����O��ۧڹ�O���@�ت֩w�A
        mpcallout 4 say �P���٥i�H��o�u�p�����~�C
        mpcallout 6 say �ڸ̦~�����Ԥh�A�פ�����ۨ����Z���C
        mpcallout 7 say �L�̳��H�ѥ[(apply)�շҬ��ؼСC
        mpcallout 8 mpdel_qmark self dhead_allmob_speaking
    endif
~
>talk_prog apply~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) !& dhead_allquest 100008
            if race($n) == 1024
                if is_qmark($n) != dhead_instructor_contestquest
                    if level($n) >= 36
                        if is_qmark(self) == dhead_instructor_bewaiting
                            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                            mpcallout 1 say ��p�A��観�H���W�ѥ[�F
                            mpcallout 2 say �е��ߤ@�I�A�ӡC
                            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                        else
                            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                            mpcallout 1 echo $I�J�Ӧa���q��$n�C
                            mpcallout 3 tsk $n
                            mpcallout 4 say �p�G�A�n�ѥ[�շ�...���o�����o���~��C
                            mpcallout 5 say �h��غ��J(corecle)�a�I
                            mpcallout 6 say �ڪ��n�B�ͷ|�i�D�A�ӫ�򰵪��C
                            mpcallout 7 wave $n
                            mpcallout 8 mpcall dhead_instructor_quest1 $n
                            mpdel_qmark self dhead_allmob_speaking
                        endif
                    else
                        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                        mpcallout 1 echo $I�J�Ӧa���q��$n�C
                        mpcallout 3 shake $n
                        mpcallout 4 say �]�\�A���x�ѻP�i��L�H�A�����m�W�����٬O�����C
                        mpcallout 5 say ���F�w���W���Ҷq�A�ڤ�����C
                        mpcallout 6 say ���A��o��h��Ըg���A�ӧa�C
                        mpcallout 7 addoil $n
                        mpcallout 8 mpdel_qmark self dhead_allmob_speaking
                    endif
                else
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 ? $n
                    mpcallout 2 say �A���O�w�g���W�ѥ[�ܡH
                    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                endif
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 say ��p�A���շҥu���h�i�H���ڤH�i�H�ѥ[�C
                mpcallout 2 say ���ڤH�Ц^�h�a�C
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say �A���O�w�g�q�L�շҤF�ܡH
            mpcallout 2 giggle
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog ready~
    if is_qmark(self) != dheah_allmob_speaking
        mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
        if pcnum($50127) > 0
          or pcnum($50128) > 0
          or pcnum($50129) > 0
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 shake $n
            mpcallout 2 say �ثe���H���b�i��շҡC
            mpcallout 3 say �еy��A�ӡC
            mpcallout 4 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) & dhead_instructor_contestquest 100032
                mpcall dhead_instructor_quest2 $n
            else
                slap $n
            endif
        endif
    endif
~
|