>repop_prog 100~
mpadd_qmark self poker_blackjack_dealer -1 0 0 0 0 1
mpadd_qmark self poker_random_timer -1 0 0 0 0 1
mpadd_qmark self poker_21_point -1 0 0 0 1 0
mpadd_qmark self poker_21_first -1 0 0 0 1 0
mpadd_qmark self poker_21_second -1 0 0 0 1 0
mpadd_qmark self poker_21_thrid -1 0 0 0 1 0
mpadd_qmark self poker_21_fourth -1 0 0 0 1 0
mpadd_qmark self poker_21_fifth -1 0 0 0 1 0
mpforcer here def_rembch $i
~
>defun_prog poker_21_check~
if is_qmark($i) == poker_21_second 100000
	mpnonechoat $n $I��W���@�i�t�P�Τ@�i
else
	mpnonechoat $n $I��W���P���G
endif
if is_qmark($i) > poker_21_first 100000
	if is_qmark($i) == poker_21_first 200001
		mpnonechoat $n [1;31m����[0m
	endif
	if is_qmark($i) == poker_21_first 200002
		mpnonechoat $n [1;31m���[0m
	endif
	if is_qmark($i) == poker_21_first 200003
		mpnonechoat $n [1;30m�®�[0m
	endif
	if is_qmark($i) == poker_21_first 200004
		mpnonechoat $n [1;30m����[0m
	endif
	if is_qmark($i) & poker_21_first 100001
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100002
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100004
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100008
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100016
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100032
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100064
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100128
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100256
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 100512
		mpnonechoat $n [1;37m����[0m
	endif
	if is_qmark($i) & poker_21_first 101024
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 102048
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_first 104096
		mpnonechoat $n [1;37m��[0m
	endif
endif
if is_qmark($i) > poker_21_second 100000
	mpnonechoat $n  �B 
	if is_qmark($i) == poker_21_second 200001
		mpnonechoat $n [1;31m����[0m
	endif
	if is_qmark($i) == poker_21_second 200002
		mpnonechoat $n [1;31m���[0m
	endif
	if is_qmark($i) == poker_21_second 200003
		mpnonechoat $n [1;30m�®�[0m
	endif
	if is_qmark($i) == poker_21_second 200004
		mpnonechoat $n [1;30m����[0m
	endif
	if is_qmark($i) & poker_21_second 100001
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100002
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100004
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100008
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100016
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100032
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100064
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100128
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100256
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 100512
		mpnonechoat $n [1;37m����[0m
	endif
	if is_qmark($i) & poker_21_second 101024
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 102048
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_second 104096
		mpnonechoat $n [1;37m��[0m
	endif
endif
if is_qmark($i) > poker_21_thrid 100000
	mpnonechoat $n  �B 
	if is_qmark($i) == poker_21_thrid 200001
		mpnonechoat $n [1;31m����[0m
	endif
	if is_qmark($i) == poker_21_thrid 200002
		mpnonechoat $n [1;31m���[0m
	endif
	if is_qmark($i) == poker_21_thrid 200003
		mpnonechoat $n [1;30m�®�[0m
	endif
	if is_qmark($i) == poker_21_thrid 200004
		mpnonechoat $n [1;30m����[0m
	endif
	if is_qmark($i) & poker_21_thrid 100001
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100002
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100004
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100008
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100016
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100032
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100064
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100128
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100256
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 100512
		mpnonechoat $n [1;37m����[0m
	endif
	if is_qmark($i) & poker_21_thrid 101024
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 102048
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_thrid 104096
		mpnonechoat $n [1;37m��[0m
	endif
endif
if is_qmark($i) > poker_21_fourth 100000
	mpnonechoat $n  �B 
	if is_qmark($i) == poker_21_fourth 200001
		mpnonechoat $n [1;31m����[0m
	endif
	if is_qmark($i) == poker_21_fourth 200002
		mpnonechoat $n [1;31m���[0m
	endif
	if is_qmark($i) == poker_21_fourth 200003
		mpnonechoat $n [1;30m�®�[0m
	endif
	if is_qmark($i) == poker_21_fourth 200004
		mpnonechoat $n [1;30m����[0m
	endif
	if is_qmark($i) & poker_21_fourth 100001
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100002
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100004
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100008
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100016
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100032
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100064
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100128
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100256
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 100512
		mpnonechoat $n [1;37m����[0m
	endif
	if is_qmark($i) & poker_21_fourth 101024
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 102048
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fourth 104096
		mpnonechoat $n [1;37m��[0m
	endif
endif
if is_qmark($i) > poker_21_fifth 100000
	mpnonechoat $n  �B 
	if is_qmark($i) == poker_21_fifth 200001
		mpnonechoat $n [1;31m����[0m
	endif
	if is_qmark($i) == poker_21_fifth 200002
		mpnonechoat $n [1;31m���[0m
	endif
	if is_qmark($i) == poker_21_fifth 200003
		mpnonechoat $n [1;30m�®�[0m
	endif
	if is_qmark($i) == poker_21_fifth 200004
		mpnonechoat $n [1;30m����[0m
	endif
	if is_qmark($i) & poker_21_fifth 100001
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100002
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100004
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100008
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100016
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100032
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100064
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100128
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100256
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 100512
		mpnonechoat $n [1;37m����[0m
	endif
	if is_qmark($i) & poker_21_fifth 101024
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 102048
		mpnonechoat $n [1;37m��[0m
	endif
	if is_qmark($i) & poker_21_fifth 104096
		mpnonechoat $n [1;37m��[0m
	endif
endif
mpechoat $n .
~
>rand_prog 100~
if is_qmark($i) == poker_blackjack_dealer 100001
	if is_qmark($i) == poker_random_timer 100002
		mpforcer here rcall poker_21_timeline $i
		mpmod_qmark self poker_random_timer v0 = 0
	else
		if rand(80)
			if rand(90)
				mpforcer here rcall poker_21_timeline $i
			else
				mpmod_qmark self poker_random_timer v0 + 1
			endif
		else
			if rand(50)
				mpforcer here rcall poker_21_timeline $i
			else
				mpmod_qmark self poker_random_timer v0 + 1
			endif
		endif
	endif
endif
~
>talk_prog topic~
tell $n �Q�F�ѥ�����G�Q�@�I�����k�ܡH(blackjack-rule)
~
>talk_prog p blackjack-rule~
tell $n ������G�Q�@�I���W�h��²��A�u�n��P�����I�Ƥ��W�L 21 �I�B�j����a�N��ӧQ�C
tell $n �u�n���������w�X�U�`�N��[�J�C���A��k�O��J bet '�Ʀr chip'�A�ثe�Ʀr�� 1 �� 10�A�Ф��n��J����C
tell $n �[�J�C������a�|�o�����a��i���}���P�A�åB�N���@�q�̫�U�`�ɶ��A�ɶ����Ҧ��H����U�`�C
tell $n �����̫�U�`��A���a�|�o���ۤv��i�P�A�Ĥ@�i�O�\�W���t�P�t�i�h�O���}�����P�A�M��N�������a�@�q��ܮɶ��C
tell $n �H�ɯ�H check card �� check dealer �Ӭݤ�W���P�β��x��W���P�C
tell $n -------------------------------------------
tell $n �b��ܮɶ����A���a���ܬO�_�n�[�`�i��@�����P(splitcard)�Χ뭰(surrender)�C
tell $n ���P�G���a�b�o�P������ۦP�Ʀr����P�ɡA�i�H�άۦP���`�X�N��i�P���}�U����`�A�̦h�i��@���C
tell $n ���X�Ӫ��P�P�쥻���P��U�|�A�U�ɤ@�i�P�A�åB�H�ɯ�H check card2 �Ӭݤ�W�����P�C
tell $n �뭰�G�b���P�Υ[�P�e�A�i�H���������Ӧ��C���A���N���h�@�b���`�X�C
tell $n ���Ъ`�N�A���P��뭰���P��鵹���a�A�N���h�Ҧ��`�X���|�k�٤@�b�C
tell $n -------------------------------------------
tell $n ��ܮɶ�������A���a�|½�}�t�P�A���ɷ|���[�P�ɶ��A���a�i�H�n�D�[�P(hit)�̦h��ɨ� 5 �i�C
tell $n ������[�P�W�h�L���ᶶ�ǡA�B�[�P�Ҭ����}�����P�A�u�n�b�@�w�ɶ������i�V��[�P�C
tell $n �p���ݭn�[�P�A�u�n���[�P�ɶ������N��A��������ݭn�S�O�۰��P�C
tell $n -------------------------------------------
tell $n �����[�P�ɶ���A���a���I�Ʀp�G���W�L 17 �I�A�h�ܤ֭n�[�P�ܤj�󵥩� 17 �I�C
tell $n �������a�ɵP��A�p�G�S�W�L 21 �z�P�A�h�}�l�p��ӭt(blackjack-rule2)�C
~
>talk_prog p blackjack-rule2~
tell $n ������G�Q�@�I���ӭt�p��W�h�G
tell $n ���a�P���I�Ƥ���a�j�ɡA�h���a�i�H��o�B�~ 1 �����`�X�C
tell $n �Ϥ��A���a�񶢮a�P���j�ɫh�|���h�Ҧ��`�X�C
tell $n ���a�P���a�P�I�ɥ���A���a�i�H���^�`�X�C
tell $n ����a�����I�ƶW�L 21 �z�P�ɡA�Ҧ����a���i�H��o�B�~ 1 �����`�X�C
tell $n -------------------------------------------
tell $n ��������S��W�h�G
tell $n ���a�b�[�P�e����ì�o 21 �I�A�h����Ĺ�o�ӳ��C������o 1.5 �����`�X�C
tell $n �����P�����p�U�ɵP��o 21 �I���|�����P�w���a�ӧQ�C
tell $n �B�Ӷ��a�N�n����ӳ��C���}�l��~��i��U�@���C���C
tell $n ���a�b½�}�t�P����o 21 �I�h����Ĺ�o�C���A�Ѿl�����a�N�����߱��`�X�C
tell $n -------------------------------------------
tell $n ���a�b�[�P����� 5 �i�P�åB��n���� 21 �I�A�BĹ�L���a�h�i�H��o�B�~ 2 �����`�X�C
tell $n ���p�G���� 5 �i�P�P���a�P�I�A�h�⥭��C
tell $n �t�~�Ъ`�N�A�U�`��p�G���a�]�U�ح�]�q�b�O��e�����A�h�������a�L����鱼�U�`�w�X�C
tell $n �p�G��F�ѥH�W���e�A�i�H�ݬݵP���I�ƪ��p��(blackjack-rule3)�覡�C
~
>talk_prog p blackjack-rule3~
tell $n �ȵP���I�ƻP�ȵP�W���Ʀr�ۦP�A���O���G���B���B���B���B���B���B���B���B�����C
tell $n ��r�ȵP�������ءB�ߡB�٥N��� 10 �I�A�� �� ��� 1 �I�� 11 �I�C
tell $n ���a�o�P����o���@ 10 �I�P�� �� �P�A�Y��o�� 21 �I�C
tell $n -------------------------------------------
tell $n �ӯȵP�����b���C�������v�T�����I�Ƥγӭt�A�Ȧ���߮��ت��\�ΡC
tell $n ���~������P�����C�֤@�i�N�|�ɶi�@�i�ۦP���P�í��s�~�P�A�i�H���P�����L���ƵP�C
tell $n �t�~����O���I����W�j�p���P���w�X(chips)�C
tell $n �p�G��W�h�S������ðݡA�h�i�H�U�`(bet)�i��C���C
~
>talk_prog chips~
tell $n �w�X�ئ@�����ح��B���O�O 100�A50�A25�A10 �� 5�C
tell $n �p�G�Q���W�Ҧ��p�B�w�X�����j���A������ڻ�(change bigger)�C
tell $n �|�ҡG���|�� 10 �Υ|�� 25 ���B���S�X�A�i�D�� change bigger�C
tell $n �ڱN�����A�@�� 100 �A�@�� 25 �A�@�� 10 �� �@�� 5 ���B���w�X�C
tell $n �p�G�Q��S�w���B���w�X�����̤j���B���A�i�H������ڻ�(change �w�X���B)�C
tell $n �|�ҡG������ 25 ���B���S�X�A�i�D�� change 25�C
tell $n �ڱN�����A�@�� 100 �Τ@�� 50 ���w�X�C
tell $n �p�G�Q��S�w���B���w�X�����S�w���B���A�i�H������ڻ�(change �����w�X���Bto�S�w�w�X���B)�C
tell $n �|�ҡG������ 25 ���B���S�X�A�i�D�� change 25to100�C
tell $n �ڱN�����A�@�� 100 �Τ@�� 25 ���w�X�C
tell $n �άO���@�� 100 ���S�X�Q������� 50 ���w�X�A�i�H�i�D�� change 100to50�C
tell $n �N��A������ƶq�����A�ڤ]�|���A�~�X�C
tell $n ���O�p�G���X���O 100to100�A50to50�Achange 100�������L��n�D�C
tell $n �ڬO���|�z�A���C
~
>talk_prog p change bigger~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall chips_count $n
mpforcer here rcall chips_change_all $n
tell $n �Ҧ��w�X�I�������C
~
>talk_prog p change 100to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_50 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 100to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 100_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 50to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 50_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 25to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 25_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_10 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 10to5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 10_chip_count $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to100~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_100 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to50~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_50 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to25~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_25 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
>talk_prog p change 5to10~
if is_qmark($n) == casino_chips_count
	break
endif
mpforcer here rcall 5_chips_count $n
mpforcer here rcall change_to_10 $n
mpforcer here rcall change_to_5 $n
tell $n �w�X�I�������C
mpdel_qmark $n casino_chips_count
~
|