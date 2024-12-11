>defun_prog dhead_jean_question~
    mpechoat $n ----  �߰ݳo�@�a������(information)
    mpechoat $n ----  ���߬ê��˶�(wound)
    mpechoat $n ----  �M�D�@�Ǹɵ��~(supplies)
    if is_qmark($n) == dhead_uruk_merchantquest 100002
        mpechoat $n ----  ����Q�|�J(uruk)���e�U
    endif
    if is_qmark($n) == dhead_uruk_merchantquest 100003
        mpechoat $n ----  ����F�z����(desert sun)
    endif
~
>repop_prog 100~
    mpoload 50010 20
    mpoload 50010 20
    mpoload 50012 20
    mpoload 50013 20
    mpat 50000 wear scimitar
    mpat 50000 wear scimitar
    mpat 50000 wear khaki
    mpat 50000 wear jeans
    mpgroup roda-dhead $i 1 0
~
>greet_prog 100~
    if isqmark(self) != dhead_jean_greeting
        mpadd_qmark self dhead_jean_greeting 1 0 0 0 1 0
        smile $n
        mpcallout 3 mpdel_qmark self dhead_jean_greeting
    endif
~
>fight_prog 100~
    if isaffected(self) $ 1
        mpdis_magic self blindness
        mpdis_magic self 'sun flash'
    endif
    if rand(33)
        say �i�c���å�...�ݩۡI
        disarm
        mpnowait
        flaming
        mpnowait
    else
        if rand(50)
        say �i�c���å�...�ݩۡI
        circular
        mpnowait
        circular
        mpnowait
        endif
    endif
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) !& dhead_allquest 100001
            if is_qmark($n) != dhead_uruk_merchantquest
                mpadd_qmark $n dhead_uruk_merchantquest -1 0 0 0 0 1
            endif
            if is_qmark($n) == dhead_uruk_merchantquest 200000
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 smile
                mpcallout 2 say �A�n�A�b�U�O�Ӧۥv�d�Ԩ����_�I�̡C
                mpcallout 3 say �ͩm���C�S...�Хs�ڬ�(jean)�N�i�H�F�C
                mpcallout 4 mpecho $I�J�ӥ��q��$n
                mpcallout 6 say �U��Ө�o��...�аݦ�����ƹ�?
                mpcallout 7 mpcall dhead_jean_question $n
                mpcallout 7 mpmod_qmark $n dhead_uruk_merchantquest v1 = 1
                mpcallout 8 mpdel_qmark self dhead_allmob_speaking
            else
                if is_qmark($n) == dhead_uruk_merchantquest
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            	    mpcallout 1 say ...�٦�����ƶ�?
            	    mpcallout 2 mpcall dhead_jean_question $n
            	    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                endif
            endif
        endif
    endif
~
>talk_prog information~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) >= dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  ponder
            mpcallout 3  say �p�A�Ҩ��A�o�̬O�F��j�F�z�������C
            mpcallout 4  say ���n��e�i�A�|��F�@���F�a...
            mpcallout 5  say ���̨�B���O���a��(ankheg)�������C
            mpcallout 6  ponder
            mpcallout 8  say �q�L�F�a��A�|��F�@���s�j�������C
            mpcallout 9  say ť��...½�L�Y�Ӯl����A�i�H���h�i�H�n��C
            mpcallout 10 say �o�@�a�����p�j���N�O�o�ˤF�C
            mpcallout 12 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog ankheg~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) >= dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 nod $n
            mpcallout 2 say ���a�η|�I��b�a���U�A�γ��������g�L�����H�C
            mpcallout 3 say �������J�������H�A�ܤ��٨Sť�������٪�...
            mpcallout 4 glare
            mpcallout 5 mpecho $I���G��M�Q�줰��A�S�X�@�ƷP������ߪ����C
            mpcallout 7 say �t�~...�q���̨��W���o�X�����...��b�����n�D�C
            mpcallout 8 say �`���A�аO�o�έ���(fly)�P����(invis)�q�L���̡C
            mpcallout 9 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog wound~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) & dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say ...�o�O�Q�t�ۥ��H(gravel giant)�`���C
            mpcallout 2 sigh
            mpcallout 4 say �a�I�N�b�n�誺�Y�Ӯl��...���̮I��ۤ@�泌�H�C
            mpcallout 5 say �L�̪�����N��P�򪺩��ۤ@��...����oı�ɤw�g�Q�L�̥]��F�C
            mpcallout 6 say �̥i�c���O...�L�̷|�ݦb���B�A�θ��ۧ������L���H�C
            mpcallout 7 say �p�G�A�̭n�g�L�����l���A�̦n����I�C
            mpcallout 8 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog supply~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) & dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say ���έ����ܡH
            mpcallout 2 say �ڭ̳o�̦��ܦh�w�s��C
            mpcallout 3 say �p�G������ݭn�A�����ݧڨ���o�Ӥp�B�ͧa�C
            mpcallout 4 mpforce roda-dhead say �ڤ~���O�p�B�ͰǡI
            mpcallout 5 mpforce roda-dhead pout
            mpcallout 6 smile
            mpcallout 7 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog uruk~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_uruk_merchantquest 100002
            if is_qmark($n) == dhead_uruk_merchantquest 200000
                mpmod_qmark $n dhead_uruk_merchantquest v1 = 1
            endif
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpmod_qmark $n dhead_uruk_merchantquest v0 = 3
            mpmod_qmark $n dhead_uruk_merchantquest v2 = 1
            mpcallout 1  say ��Ӧp���A�Q�|�J�w�g�N���ȩe�U�A�F�ܡH
            mpcallout 2  ponder
            mpcallout 4  say ...���Q�|�J�O�ӰӤH�A�L�u�Ш����_�I�̪��ڦP��C
            mpcallout 5  say �ڭ̦�����{���ت��A�O���F�M��ǻ������F�z����(desert sun)...
            mpcallout 6  say �L�`�a�A�b�~���ڭ̨��L�Y�F...�~���J���H���l��
            mpcallout 7  mpecho $I���ۻ���...�}�l�K�_�F���Y�C
            mpcallout 9  say �b���H��ŧ���U...�ڨ��F����...��ӭp���]�]���Q�Ի~�F...
            mpcallout 10 mpecho $I��_�Y�ӡA�δ��ݪ��������$n�C
            mpcallout 12 say �J�M�Q�|�J�w�g�N�u�@�e�U���A...���N�U�ƫ��U�F�C
            mpcallout 13 say �аݧڭ̴M��F�z�����A�N�L�浹�Q�|�J(uruk)�C
            mpcallout 14 bow $n
            mpcallout 15 mpdel_qmark self dhead_allmob_speaking
        else
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 ? $n
            mpcallout 2 say ...���Ƨ�Q�|�J�ܡH
            mpcallout 3 say ...�L�K�����b�b�ץ~�������ۨ��̡C
            mpcallout 4 say ������Ƥ����˦ۥh��L���H�a�H
            mpcallout 5 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog desert sun~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_uruk_merchantquest 100003
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say �F�z����(desert sun)
            mpcallout 2  say �o�ӦW�r���ӷ�...�O�F�z�����j�Ѫ��ǻ��C
            mpcallout 3  say ���O�@�عڤۤ����ī~...�ڻ��i�H��v�ʯf�C
            mpcallout 4  say �ר��Y�ؤ��v���g�����f�A������H�����ġC
            mpcallout 5  mpecho $I�`�`�a�l�F�@�f��C
            mpcallout 7  say �ڭ̫�ݭn��...�i�O������������b�ܦ����C
            mpcallout 8  say �̷s���u���A���X�n���F�z�n���~��j���C
            mpcallout 9  ponder
            mpcallout 11 say �u�O��p...�ڪ��D���u���o�ǤF�C
            mpcallout 12 mpdel_qmark self dhead_allmob_speaking
        else
            ? $n
        endif
    endif
~
|