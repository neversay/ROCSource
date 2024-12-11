>defun_prog dhead_corecle_attack~
    if isfight(self)
        break
    else
        say �J�M�A�����𪺾��|�A���ڴN���Ȯ�F�I
        mpkill $n
    endif
~
>defun_prog dhead_corecle_beaffected~
    if is_qmark(self) !& dhead_corecle_questing 100001
        mpmod_qmark self dhead_corecle_questing v0 | 1
    endif
    if rand(20)
        say �������å�...
    endif
~
>repop_prog 100~
    mpadd_qmark self delayed_cast -1 9999 0 0 1 0
    mpoload 50090
    mpoload 50091
    mpoload 50092
    mpoload 50093
    mpoload 50094
    mpoload 50095
    mpat 50000 wear all
~
>kill_prog 100~
    say �G�M�٬O�ӹ�...
    if is_qmark($n) == dhead_instructor_contestquest 100001
        mpat $n mpmod_qmark $n dhead_instructor_contestquest v0 = 0
    endif
    mppurge self
~
>fight_prog 100~
    if is_qmark(self) == dhead_corecle_questing
        if is_qmark($n) != dhead_instructor_contestquest 100001
            if is_qmark(self) !& dhead_corecle_questing 100002
                mpmod_qmark self dhead_corecle_questing v0 | 2
            endif
            break
        else
            if isaffected(self) & 268435457
                mpcallout dhead_corecle_beaffected $n
                break
            else
                if rand(33)
                    say �ݧڪ�...�ֳt�C
                    skilldrv multihit
                    mpnowait
                else
                    if rand(50)
                        say ���}��I
                        disarm
                        mpnowait
                    endif
                endif
            endif
        endif
    endif
~
>hitprcnt_prog 20~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark(self) == dhead_corecle_questing 100000
            if is_qmark($n) == dhead_instructor_contestquest 100001
                if rand(75)
                    mpgoto $n
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 say ���F�A�ڧ뭰�F�I
                    mpcallout 2 clap $n
                    mpcallout 4 say �A����{�۷��u���A���o���ħ����Ӥ��ܩ���`���C
                    mpcallout 5 say �A�w�g���o���F�A�^�v�޳��ѥ[�շҧa�I
                    mpcallout 6 addoil $n
                    mpcallout 6 mpmod_qmark $n dhead_instructor_contestquest v0 | 24
                    mpcallout 7 mppurge self
                 endif
            endif
        endif
    endif
~
>death_prog 100~
    if is_qmark(self) == dhead_corecle_questing
        if is_qmark($n) == dhead_instructor_contestquest 100001
            mpdis_magic self all
            if is_qmark(self) == dhead_corecle_questing 100000
                say �X��u��...�A�w�g���o���F�I
                mpmod_qmark $n dhead_instructor_contestquest v0 | 20
                break
            else
                say ���M�ΤF�ܤ����a����q...���A�٬O���o���F�I
                mpmod_qmark $n dhead_instructor_contestquest v0 | 18
            endif
        endif
    endif
~
>greet_prog 100~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark(self) == dhead_corecle_questing
            mpkill $n
            break
        else
            if is_qmark($n) == dhead_instructor_contestquest 100000
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1  hmm
                mpcallout 2  say �Q�n�ѥ[�շҪ��s�H�N�O�A�ܡH
                mpcallout 3  mpecho $I�J�Ӧa���q��$n�C
                mpcallout 5  tsk $n
                mpcallout 6  mpecho $I�p�n�a���ۡG��������...�q�~��Ӭݲ������`�C
                mpcallout 8  say �y...�y...
                mpcallout 9  say ť�n...�A�n�����o�ѥ[�շҪ����C
                mpcallout 10 say ����F�N�O��ڥ��@�[�A�ҩ��A����O�C
                mpcallout 11 hmm $n
                mpcallout 12 say �`�N�F...�ڷ|�b��Ԥ���A����{���X���q�C
                mpcallout 13 say �o�]�|�C�J�N�ӸշҪ�����������C
                mpcallout 14 say �p�G�Aı�o�ǳƦn�F�A�N��ڻ��n(ready)�C
                mpcallout 14 mpmod_qmark $n dhead_instructor_contestquest v0 = 1
                mpcallout 15 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>talk_prog ready~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_instructor_contestquest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpdis_magic self all
            mpat 50000 c 'full heal'
            mpat 50000 c 'detect invis'
            mpat 50000 c 'detect hidden'
            mpcallout 1 say �n�A���ڭ̶}�l�a�C
            mpcallout 2 say �A�i�H���X��(kill corecle)�C
            mpcallout 2 mpadd_qmark self dhead_corecle_questing -1 0 0 0 1 0
            mpcallout 2 mpmod_qmark $n dhead_instructor_contestquest v0 = 2
            mpcallout 4 mpcall dhead_corecle_attack $n
            mpcallout 5 mpdel_qmark self dhead_allmob_speaking
         endif
    endif
~
|