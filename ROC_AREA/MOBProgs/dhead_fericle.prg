>defun_prog dhead_fericle_givedrink~
    mpoload 50130 $n 1 target
    mpechoat $n $I���A�ˤF�@�M�s�C
    mpforce $n drink wine
~
>defun_prog dhead_fericle_givereward~
    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
    mpmod_qmark $n dhead_fericle_winequest v0 = 3
    mpmod_qmark $n dhead_fericle_winequest v1 = 0
    mpcallout 1 say �Ӧn�F�I�T��h�i�ֳ�����F�C
    mpcallout 3 say �o�O���A���S���C
    mpcallout 4 mpgold $n 100000
    mpcallout 4 mpechoat $n $I���F�A�@�U�����A�̭����Q�U�����C
    mpcallout 4 mpechoaround $I���F $n �@�U�����C
    mpcallout 6 �t�~�������@�U�A���ڽЧA�ܤ@�M�a�C
    mpcallout 7 mpcall dhead_fericle_givedrink $n 
    mpcallout 8 giggle
    mpcallout 8 mpdel_qmark self dhead_allmob_speaking
~
>defun_prog dhead_fericle_randprog~
    if pcnum($) > 0
        if is_qmark(self) != dheah_allmob_speaking
            if is_qmark($r) == dhead_fericle_winequest 300015
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpdel_qmark $r dhead_fericle_winequest
                if is_qmark(%r) != dhead_allquest
                    mpadd_qmark $r dhead_allquest
                endif
                mpmod_qmark $r dhead_allquest v0 | 2
                mpcallout 1 say �u���ܷP�§A�������C
                mpcallout 3 say �o�O���A���S���C
                mpcallout 4 mpgold $r 1000000
                mpcallout 4 mpechoat $n $I���F�A�@�U�����A�̭����@�ʸU�����C
                mpcallout 4 mpechoaround $I���F $n �@�U�����C
                mpcallout 4 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>defun_prog dhead_fericle_gotwine~
    if is_qmark($n) < dhead_fericle_winequest 300015
        say �٦���L���s�ܡH
        giggle
    endif
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) & dhead_allquest 2
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 hmm
            mpcallout 2 say ��ˡH�n�ӶR�s�ܡH
            mpcallout 3 giggle $n
            mpcallout 5 �p�G�n�R�S�s���s
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) > dhead_fericle_winequest 300001
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 say ��ˡH
                mpcallout 2 say 
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            else
                if level($n) > 30
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 hmm
                    mpcallout 2 say �n�R�s�ܡH
                    mpcallout 3 say �~�|�ݬݥ����S�s���h�ְs(lizard wine)�A�]�g���N�I
                    mpcallout 3 mpadd_qmark $n dhead_fericle -1 0 0 0 0 1
                    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                else
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 hmm
                    mpcallout 2 say �n�R�s�ܡH
                    mpcallout 3 say �A�٤Ӧ~���A�٬O�ֳ��I�a�C
                    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                endif
            endif
        endif
    endif
~
>talk_prog lizard wind~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say ��H�h�ְs(lizard wine)�ڡC
            mpcallout 2  say �U�W��q...�N�O�λh�i���ְ����C
            mpcallout 3  say �o�ܩ_�ǶܡH
            mpcallout 4  giggle $n
            mpcallout 6  mpecho $I�������@�G�A���G��M�Q�_����ơC
            mpcallout 7  say ��F�I�w�s���h�i�֭�n�����ΤF�C
            mpcallout 8  mpechoat $n $I�J�Ӧa���q�ۧA�C
            mpcallout 8  mpechoaround $n $I�J�Ӧa���q�� $n �C
            mpcallout 10 say �o��B�ͦ��G�O�ܦ��g�窺�_�I�̡C
            mpcallout 11 say �i�_���ڥ��X�i�h�i�ֶܡH(yes/no)
            mpcallout 11 mpmod_qmark $n dhead_fericle v0 = 1
            mpcallout 11 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog yes~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            say �Ӧn�F�I
            mpcallout 1 giggle $n
            mpcallout 3 say �ڥΨ��C�s���h�i�֡A�O���۬Y�ؤj�Y�h�i���W�C
            mpcallout 4 say �o�ػh�i�u�|�b�~�������@�a�X�S�C
            mpcallout 5 say �ڻݭn�T�i�A�����Ъ����浹�ڡC
            mpcallout 6 addoil $n
            mpcallout 6 mpmod_qmark $n dhead_fericle_winequest v0 = 2
            mpcallout 6 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog no~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpdel_qmark $n dhead_fericle_winequest
            hmm
            mpcallout 1 sigh
            mpcallout 3 say ���u�O�i���C
            mpcallout 4 mpecho $I��L���h�A�~��M�z�s�M�C
            mpcallout 4 mpdel_qmark self dhead_allmob_speaking
        endif
        else
            - $n
        endif
    endif
~
>talk_prog past~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100003
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  hmm $n
            mpcallout 3  glare
            mpcallout 5  say �~�����ɭ�...�ڤ]���g�b�~���Ȧ�L�@�}�l�C
            mpcallout 6  say ��A���O��q�L�Ԥh�սm(contest)���ɭԧa�C
            mpcallout 7  say ����٬O�f�ۿˤH�A�������H�~�a���_�I�����a�X�����C
            mpcallout 8  giggle
            mpcallout 10 say �֦~�����`�O�Q�����諸����...
            mpcallout 11 say ��A���L�����O�L�h���ƤF�C
            mpcallout 12 say �p���ڥu�Q�ݦb�G�m�A�L�۰s������(bartender)���y���ͬ��C
            mpcallout 13 giggle
            mpcallout 13 mpmod_qmark $n dhead_fericle_winequest v0 = 4
            mpcallout 13 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) > dhead_fericle_winequest 100003
              or is_qmark($n) & dhead_allquest 100002
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 say �L���p��A�{�b���ڥu�O�ӱy�����s������(bartender)�Ӥw�C
                mpcallout 2 giggle
                mpcallout 2 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 dunno $n
                mpcallout 2 say �ڥu�O�ӥ��Z���հs�v�Ӥw�C
                mpcallout 3 giggle
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>talk_prog bartender~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100004
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say �o�N�n�q�ڦb�~���_�I�����q��l���_�F�C
            mpcallout 2  say ��ɪ��ڸ�ۤ@���նħL�ήȦ�A���g���X�L�\�h�a��C
            mpcallout 3  say �q�ۦW���v�d�Ԩ����A�������B�P��S���A
            mpcallout 4  say �Ʀܻ��筫�v��������A�����ڪ�����C
            mpcallout 5  say ��M�A�C��@�ӷs������A�N�|�Ǿ��~�|�L��a�S�����s�C
            mpcallout 6  giggle
            mpcallout 8  say �����a��~�s�B�C�s�P�쿳��A�]�Ǩ줣�֧ޥ��C
            mpcallout 9  say ��ӶħL�θѴ��F�A���q�_�I�ͲP�]��۵����F�C
            mpcallout 10  say ��O�N�Q�^�G�m�A�Q�ΩҾǦ����s���޳N�}�@���s�a�C
            mpcallout 11 say ���R�ܰs(winebibbing)���ڤH�]��~�|��@�ɦU�a���W�s�C
            mpcallout 12 giggle
            mpcallout 12 mpmod_qmark $n dhead_fericle_winequest v0 = 5
            mpcallout 12 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog winebibbing~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100005
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say �h�i�H�ڡA�O�@�ӥX�W�a�R�ܰs���رڡC
            mpcallout 2 say �Y�ϳܰs�L�h�|�����g�A�`�`�R��(acid)�R�o��B���O�C
            mpcallout 3 shrug
            mpcallout 5 mpecho $I��M�C�U�Y�ӡA���G�Q�줰��дo���ơC
            mpcallout 7 say ���L�A�ƹ�W...����Ƨڤ@��ı�o�ܿ��(regret)�C
            mpcallout 7 mpmod_qmark $n dhead_fericle_winequest v0 = 6
            mpcallout 7 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog regret~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100006
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say ���M�s�����ȤH�H��b�o�̪��h�i�H���D�A
            mpcallout 2  say �����M�]�|���Ӧۥ~�a���_�I�̩έx�x�C
            mpcallout 3  glare
            mpcallout 5  say �L�H�Z�`�誺�A���@��_�˲��A���H���C�h�C
            mpcallout 6  say �L�޵۩_�S���f���A�ۺ٬O�Ӧۤ@�ӥs�M�e���a��C
            mpcallout 7  say �٦��@����ڪ��_�I�̡A���g����L�b�H�������g���C
            mpcallout 8  say ���O�ӧϩ��u�X�{�b�ǻ������a��C
            mpcallout 9  ponder
            mpcallout 11 say �o��Ӧa��ڲ���ť�D�A�Q����a�����S�����s�C
            mpcallout 12 say �i���L�h�_�I�ͲP�A�ڱq���˦۽񨬳o��Ӧa��C
            mpcallout 13 say �{�b���۰s�a����~�A���ӥi��h�~�|�ݬݷ�a���s�F�C
            mpcallout 14 say �p�G�A�����|�g�L�A�O�ѤF���ڱa�@�ǰs�^�ӡC
            mpcallout 15 say �ڷ|�n�n�S�§A���C
            mpcallout 16 giggle
            mpcallout 16 mpmod_qmark $n dhead_fericle_winequest v0 = 7
            mpcallout 16 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>give_prog 50060~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100002
            mpmod_qmark $n dhead_fericle_winequest v1 + 1
            mpjunk $o
            if is_qmark($n) == dhead_fericle_winequest 200001
                say �ڦ���Ĥ@��h�i�֤F�C
            else
                if is_qmark($n) == dhead_fericle_winequest 200002
                    say �ڦ���ĤG��h�i�֤F�C
                else
                    mpcall dhead_fericle_givereward $n
                endif
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 11114~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300001
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1 say ��...�o�O�M�e���˸��C�s�C
                mpcallout 2 mpecho $I�����ⱷ�۰s�~�A�N��l�w�w�a���i�~�f...
                mpcallout 3 mpecho �H���S�X�@�ƫܳ��K�����C
                mpcallout 5 mpecho $I���a���ۡG�s���q�~�l���ƤF�X��...�@�طL�H���P�񺡤F�ڪ����ߡC
                mpcallout 7 say �n�s�I�n�s�I
                mpcallout 7 mpmod_qmark $n dhead_fericle_winequest v2 | 1
                mpcallout 8 mpcall dhead_fericle_gotwine $n
                mpcallout 8 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say �o���O�M�e���˸��C�s�ܡH
                mpcallout 2 say �ڤw�g�~�|�L�F�ڡC
                mpcallout 3 give $o $n
                mpcallout 4 shrug
                mpcallout 4 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 11128~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300002
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1 say ��...�o�ذs�s���п��s�ڡC
                mpcallout 2 say ���ڨӥJ�Ӧa�~�|�ݬݡC
                mpcallout 3 mpecho $I���X�@�u�p�s�M�A�w�w�a�ˤJ�F�ֶq���п��s...
                mpcallout 5 mpecho �u���L�ӲӦa���F�@�f�A�ߧY�S�X�@�ƫܳ��K�����C
                mpcallout 6mpecho $I���a���ۡG���\�O�b�ʦѰs�A�G�M���J�i�f�C
                mpcallout 8 say �n�s�I�n�s�I
                mpcallout 9 mpmod_qmark $n dhead_fericle_winequest v2 | 2
                mpcallout 9 mpcall dhead_fericle_gotwine $n
                mpcallout 9 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say ��A�o���O�M�e���п��s�ܡH
                mpcallout 2 say �ڤw�g�~�|�L�F�ڡC
                mpcallout 3 give $o $n
                mpcallout 4 shrug
                mpcallout 4 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 50005~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300004
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1  say ��...�o�O�ǻ������H�����S���������s�ڡC
                mpcallout 2  tsk
                mpcallout 3  mpecho $I���X�@�u�p�s�M�A�w�w�a�ˤJ�F�ֶq�������s...
                mpcallout 5  mpecho �u���L�L���@�f�A�ߧY���y�q���A�@�Ʀ�߼Q�i���i�����C
                mpcallout 6  gasp
                mpcallout 8  mpecho $I�n�j�P���f�P�I�ϩ��L�v�j���몺��ժ`���F�ڪ������I
                mpcallout 9  say �n�s�I�n�s�I
                mpcallout 9  mpmod_qmark $n dhead_fericle_winequest v2 | 4
                mpcallout 10 mpcall dhead_fericle_gotwine $n
                mpcallout 10 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say ��A�o�O�H�����������s�a�H
                mpcallout 2 say ���ب��D�u���H�S�����ѰڡC
                mpcallout 3 say ���L�A�ڷQ�A�٬O���d�ۧa�C
                mpcallout 4 give $o $n
                mpcallout 5 giggle
                mpcallout 5 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 50008~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300008
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1  say ��...�A���ڱa�ӤH�����S��������s�ܡH
                mpcallout 2  giggle
                mpcallout 4  mpecho $I�N��l����s�~�A������J�@�f�s��...
                mpcallout 5  mpecho �u���L���۰s�~�V��V�`�A�H�Y�}�l�L�|�~�����s�C
                mpcallout 6  mpecho �A�ݨ�$I�ϩ����]�����A���M�����j�f�j�f�a�ܰ_�s�ӡC
                mpcallout 7  mpecho �b$I���������U�A�ܧ֦a��Ӱs�~�Q�ܱo�w�s���ѡC
                mpcallout 9  mpecho $I�@�y�_��a�o�X�n���G��...
                mpcallout 10 say �ڳ��M�b������ı�����p�U�A�ܰ��F��~�s�I
                mpcallout 11 say �n�s�I�n�s�I
                mpcallout 11 mpmod_qmark $n dhead_fericle_winequest v2 | 8
                mpcallout 12 mpcall dhead_fericle_gotwine $n
                mpcallout 12 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say ��A�o�O�H����������s�a�H
                mpcallout 2 say �o�ذs�n�ܦa���H�`�O�@���ӺɡC
                mpcallout 3 say ���L�A�ڷQ�A�٬O���d�ۧa�C
                mpcallout 4 give $o $n
                mpcallout 5 giggle
                mpcallout 6 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog all~
    drop all
~
|