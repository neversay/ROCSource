>repop_prog 100~
mpadd_qmark self roulette_dealer -1 0 0 0 0 1
mpadd_qmark self roulette_random_timer -1 0 0 0 0 1
mpforcer here def_rembch $i
~
>rand_prog 100~
if is_qmark($i) == roulette_dealer 100001
	if is_qmark($i) == roulette_random_timer 100002
		mpforcer here rcall roulette_timeline $i
		mpmod_qmark self roulette_random_timer v0 = 0
	else
		if rand(80)
			if rand(90)
				mpforcer here rcall roulette_timeline $i
			else
				mpmod_qmark self roulette_random_timer v0 + 1
			endif
		else
			if rand(50)
				mpforcer here rcall roulette_timeline $i
			else
				mpmod_qmark self roulette_random_timer v0 + 1
			endif
		endif
	endif
endif
~
>talk_prog topic~
tell $n �Q�F�ѥ����L�����k�ܡH(roulette-rule)
~
>talk_prog p roulette-rule~
tell $n �u�n��۷Q�n����m�U�`�Y�i�[�J�C���A�̲׽��L���Ʀr�P�U�`�Ʀr�ۦP�h�i�H�o�줣�P�߲v�����S�C
tell $n �U�`����k�� bet '��m �Ʀr�榡'�A�C���U�`�� 1 ���S�X�A�P�˦�m�u�i�H�U�@���A�P���C�����̦h�U�` 10 ���C
tell $n �b�C�������H�ɨϥ� check �Ӭݥx�઺�˻��Υثe�ۤv�U�`�����p�C
tell $n �U�`��m���G��Ʀr(single)�B�Ʀr��(split)�B�C(street)�B��(corner)�B��(line)�B��(column)�B��(dozen)�B
tell $n �@�@�@�@�@�@�_��(odd)�B����(even)�B���㢰��(1to18)�B�����㢲��(19to36)�B��(black)�B��(red)�C
tell $n �C�ؤU�`��m�������P���Ʀr�榡�U���N�v�@�����C(roulette-rule2)
~
>talk_prog p roulette-rule2~
tell $n ��Ʀr(single)�Gbet 'single [1;37m���X[1;33m'
tell $n ����k�ઽ���U�`�b���@�Ʀr�W�A�Ӹ��X�� 0 �� 36�C
tell $n �ҡG�U�`���X 0 �h��J bet 'single 0'�A�U�`���X 3 �h��J bet 'single 3'
tell $n ------------------------------------------------------------------------------
tell $n �Ʀr��(split)�Gbet 'split [1;37m���X1[1;33m',[1;37m���X2[1;33m'
tell $n �b�۾F���⸹�X�����u�W�U�`�A��@����` 2 �ӼƦr�A���O���]�t���X 0�C
tell $n ���X1 �P ���X2 ���x��W��Ӭ۾F�����X�A���X�O�_�۾F�i�� check ���O�d�ݡC
tell $n �ҡG�U�` 1 �P 2 �����h��J bet 'split 1,2'�A�U�` 4 �P 7 �����h��J bet 'split 4,7'
tell $n ���G��Ʀr�H , �Ÿ��Ϲj�A���ǥi�H�ܧ�C
tell $n ------------------------------------------------------------------------------
tell $n �C(street)�Gbet 'street [1;37m�̤p���X[1;33m'to[1;37m�̤j���X[1;33m'
tell $n �b�Ĥ@��(column 1)���u�W���ƽu�W�U�`�A���`�@�C 3 �Ӹ��X�A�����]�t���X 0�C
tell $n �@�C���̤p���Ʀr���̤p���X�A�ӳ̤j���Ʀr�h�O�̤j���X�A�C�ӦC�]�t�����X�i�� check ���O�d�ݡC
tell $n �ҡG�U�` 1, 2, 3 ���C�h��J bet 'street 1to3'�A�U�` 25, 26, 27 ���C�h��J bet 'street 25to27'
tell $n ------------------------------------------------------------------------------
tell $n ��(corner)�Gbet 'corner down[1;37m�Ʀr[1;33m' �� bet 'corner upper[1;37m�Ʀr[1;33m'
tell $n �|�Ӹ��X����e�I�٬����A�b���U�`��@����` 4 �ӼƦr�A�����]�t���X 0�C
tell $n �x��W�@�� 22 �Ө��A�ѲĤG��(column 2)����m�Ϥ����U(down)�P�W(upper)�A�Ʀr�ѥ���k�W�[�A���O�� 1 �� 11�C
tell $n �C�Ө��]�t�����X�i�� check ���O�d�ݡC
tell $n �ҡG�U�` 1, 2, 4, 5 �����h��J bet 'corner down1'�A�U�` 29, 30, 32, 33 �����h��J bet 'corner upper10'
tell $n ------------------------------------------------------------------------------
tell $n ��(line)�Gbet 'line [1;37m�̤p���X[1;33m'to[1;37m�̤j���X[1;33m'
tell $n �b�Ĥ@��(column 1)���u�W���ƶ��u�W�U�`�A���`�@�� 6 �Ӹ��X�A�����]�t���X 0�C
tell $n �@�Ƥ��̤p���Ʀr���̤p���X�A�ӳ̤j���Ʀr�h�O�̤j���X�A�C�ӱƥ]�t�����X�i�� check ���O�d�ݡC
tell $n �ҡG�U�` 1, 2, 3, 4, 5, 6 ���ƫh��J bet 'line 1to6'
tell $n �@�@�U�` 22, 23, 24, 25, 26 ���ƫh��J bet 'street 22to27'
tell $n ------------------------------------------------------------------------------
tell $n ��(column)�Gbet 'column [1;37m���[1;33m'
tell $n ��S�w��ƤU�`��@����`�@�椤�� 12 �ӼƦr�A�����]�t���X 0�C
tell $n �C�Ӧ�]�t�����X�i�� check ���O�d�ݡC
tell $n �ҡG�U�`�Ĥ@��h��J bet 'column 1'�A�U�`�ĤG��h��J bet 'column 2'�A�U�`�ĤT��h��J bet 'column 3'
tell $n ------------------------------------------------------------------------------
tell $n ��(dozen)�Gbet 'dozen [1;37m�ĴX��[1;33m'
tell $n ��S�w���ƤU�`��@����`�@������ 12 �ӼƦr�A�����]�t���X 0�C
tell $n �C�ӥ��]�t�����X�i�� check ���O�d�ݡC
tell $n �ҡG�U�`�Ĥ@���h��J bet 'dozen 1'�A�U�`�ĤG���h��J bet 'dozen 2'�A�U�`�ĤT���h��J bet 'dozen 3'
tell $n ------------------------------------------------------------------------------
tell $n ����(even)�Gbet even�A��`�x��W���]�t 0 ���Ҧ����Ƹ��X�C
tell $n �_��(odd)�Gbet odd�A��`�x��W���]�t 0 ���Ҧ��_�Ƹ��X�C
tell $n ���㢰��(1to18)�Gbet 1to18�A��`���X 1 �� 18�C
tell $n �����㢲��(19to36)�Gbet 19to36�A��`���X 19 �� 36�C
tell $n ��(red)�Gbet red�A��`���⸹�X�A���X���C��i�� check ���O�d�ݡC
tell $n ��(black)�Gbet black�A��`�¦⸹�X�A���X���C��i�� check ���O�d�ݡC
tell $n ------------------------------------------------------------------------------
tell $n �p�G�ٷQ�F�ѩ㤤�U��m���߲v�i�H�~��ݧڡC(roulette-rule3)
~
>talk_prog p roulette-rule3~
tell $n �C���U�`�� 1 ���A�P���C�����C�춢�a�̦h�U�` 10 ���C
tell $n ---------------------------------------------
tell $n ��Ʀr(single)                    1 �� 35
tell $n �Ʀr��(split)                     1 �� 17
tell $n �C(street)                        1 �� 11
tell $n ��(corner)                        1 �� 8
tell $n ��(line)                          1 �� 5
tell $n ��(column)                        1 �� 2
tell $n ��(dozen)                         1 �� 2
tell $n ����(even)                        1 �� 1
tell $n �_��(odd)                         1 �� 1
tell $n ���㢰��(1to18)                   1 �� 1
tell $n �����㢲��(19to36)                1 �� 1
tell $n ��(red)                           1 �� 1
tell $n ��(black)                         1 �� 1
tell $n ---------------------------------------------
tell $n �t�~����O��I����W�j�p���P���w�X(chips)�C
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