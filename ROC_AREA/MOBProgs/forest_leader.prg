>greet_prog 100~
	if is_qmark($n) == forest_enemy 100000
		if is_qmark($n) != forest_quest
			mpechoat $n $I �f�������D����ۤ���F��....�I
			mpechoaround $n $I ���M���_�Z���� $n �ĤF�L�h....�I
			mpechoat $n $I ���M���_�Z�����A�ĤF�L��....�I
			mpat 5 invoke
			hunt $n
		else
			mpechoat $n $I ���G�������ݱ��ڭ̱��ت��H....�I
			mpechoat $n �d���� $I ���_�Z���V�A�ĤF�L��....�I
			mpechoaround $n $I ���M���_�Z���� $n �ĤF�L�h....�I
		endif
		break
	endif
	if is_qmark($i) == forest_fiesta 300000
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if is_qmark($n) !& forest_quest 300256
			mpechoat $n �A����F $I �A�A�߷Q�G�ӫ��}�l(start)��L���O�H
		endif
		break
	endif
	if ispc($n)
		if class($n) == 13
or class($n) == 14
or class($n) == 15
			if is_qmark($n) != forest_barbarian_exam
				if is_qmark($n) < forest_quest 100009
					mpechoat $n $I ���G�A���]�O���𪺤G�Ӷi�泥�Z�H�շҤF�ܡH
				endif
			endif
		endif
		break
	endif
~
>talk_prog topic~
	if is_qmark($i) == forest_fiesta 300000
		if is_qmark($n) != forest_quest
			if is_qmark($i) > forest_fiesta 200024
				if is_qmark($i) < forest_fiesta 200048
					mpechoat $n $I ���b�R�R����ë�A�������Q�z�A�C
				endif
			else
				mpechoat $n $I ���GAST Q ADHGDY GDYRER VHX DDY DSYTVESTV
				mpechoat $n �A����ť�����L�쩳�b���Ǥ���C
				mpechoat $n ���L�A�i�H����Pı�X�ӳo���צ~�������x��^....�C
			endif
		else
			if is_qmark($i) < forest_fiesta 200013
or is_qmark($i) > forest_fiesta 200047
				if rand(50)
					mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
					mpechoat $n $I ���G�A�n�A $n �A�n���n�@�_���ӻR....�C
					mpechoat $n ���� $I �ԵۧA����A�N���_�R�ӤF....�C
					mpechoaround $n $I �Ե� $n ������_�R�ӤF....�C
				else
					mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
					mpechoat $n $I ���G�o���O $n �a�I�w��ѥ[�ڭ̪��צ~���C
					mpechoat $n $I ���G�o�O�ڭ̬��y�ʪ����סA�ӡA�A�Y�Y��....�C
					mpoload 4253
					give leg $n
					mpforce $n eat leg
					gig
					mpechoat $n $I ���G�p��H�Pı�����a�I
				endif
				break
			endif
			if is_qmark($i) > forest_fiesta 200012
				if is_qmark($i) < forest_fiesta 200025
					if rand(50)
						mpechoat $n $I ���G�z�I�Y���n���A�A�ۤv���a�I
						gig
					else
						mpechoat $n $I �K�����٤H�ơA�A�٬O���n�ޥL�n�F�C
					endif
				endif
				break
			endif
			if is_qmark($i) > forest_fiesta 200024
				if is_qmark($i) < forest_fiesta 200048
					mpechoat $n $I �R�R���b�D����ë�������A�������z�A�C
				endif
			endif
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100010
		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		mpechoat $n $I ���G�z�n�A�ڭ��Z��˪L���^���I
		break
	endif
	if is_qmark($n) == forest_quest 100001
		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		mpechoat $n $I ���G�z�n�A�ڬO�o�Ӥg�H����������C
		mpechoat $n $I ���G�ڷQ�����o�̪��@�ǨƱ��A�A�w�g�۷��A�ѤF�C
		mpechoat $n $I ���G�ڳo�䦳�@�ǨƱ�(quest)�ݭn�A�˨��h�����A
		mpechoat $n $I ���G�p�G�A��L�̵������F�A�N�N��A���e�����s�`�B�����ɤڬ��q�C
		mpechoat $n $I ���G�ڷ|���ѧکҦ�����U�A���U�A�������s�C
		mpmod_qmark $n forest_quest v0 + 1
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) == forest_heart 100001
			 if is_inv_obj($n) == 4248
			 	mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			 	mpechoat $n $I ���G�֧�[1;36m���������`����Ŧ(heart of huge fire newt)[0m�����ڧa�I
			 endif
			 break
		endif
		if is_qmark($n) == forest_scale 100001
			 if is_inv_obj($n) == 4244
			 	mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			 	mpechoat $n $I ���G�֧�[1;36m�D�Z�����(scale of snake queen)[0m�����ڧa�I
			 endif
			 break
		endif
		if is_qmark($n) == forest_eyes 100001
			 if is_inv_obj($n) == 4245
			 	mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			 	mpechoat $n $I ���G�֧�[1;36m���������(eyes of lion king)[0m�����ڧa�I
			 endif
			 break
		endif
		if is_qmark($n) == forest_skin 100001
			 if is_inv_obj($n) == 4246
			 	mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			 	mpechoat $n $I ���G�֧�[1;36m�}�ޥ��H�����ֽ�(skin of troll king)[0m�����ڧa�I
			 endif
			 break
		endif
		if is_qmark($n) < forest_burgeon 100003
			 if is_inv_obj($n) == 4247
			 	mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			 	mpechoat $n $I ���G�֧�[1;36m���줧�������(burgeon of old tree)[0m�����ڧa�I
			 endif
			 break
		endif
		break
	endif
	if is_qmark($n) != forest_quest
		mpechoat $n $I ���GGEWR AG AEWTABTS SDY ESVEUUs SEVY QW�I
		mpechoat $n �A����ť�����L�쩳�b���Ǥ���C
	endif
~
>talk_prog identify~
	if is_qmark($n) & forest_quest 300512
		if is_qmark($n) !& forest_quest 301024
			mpecho $I ���y�W�S�X�Q���Y�ߪ����e....�C
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�ڭ̪����H�A�S�Q���ٱo�·ЧA���o������[�֪��ơC
			mpechoat $n $I ���G�ڬ۫H�A�V�m�����G�A�ڦP�N�L�̪��B�ƤF�C
			mpechoat $n �㽬�R(Alleina) �P �N�J�]Hawk) ��ӤH��򪺩�b�@�_�C
			mpechoat $n $I ���G[1;36m�n�F�A�A�̨�ӥ����}�a�I�ڦ���(words)�n�� $n ���C[0m
			mpecho �N�J �P �㽬�R ��o�ۤ��~���h�}�F....�C
			mpmod_qmark $n forest_quest v2 + 1024
			mptransfer hawk
			mppurge hawk
			mptransfer alleina
			mppurge alleina
		endif
	endif
~
>talk_prog words~
	if is_qmark($n) & forest_quest 301024
		mpechoat $n $I �� �N�J �P �㽬�R �����}�F����A���M�V�A���F�U�ӡC
		mpechoaround $n $I �� �N�J �P �㽬�R �����}�F����A���M�V $n ���F�U�ӡC
		mpechoat $n �A�s���N $I ���ߤF�_��....�C
		mpechoround $n $n �s���N $I ���ߤF�_�ӡC
		mpechoat $n $I ���G���¡A�s�ڤk�઺�ץͤj�Ƴ��n�·бz�A�u�O....�C
		mpechoat $n $I ���G�ڳ������D���P�±z�F�C
		mpechoat $n [1;32m�A�߷Q�G�O�_���ӰݰݥL�̦���|��B§(marriage)�O�H[0m
	endif
~
>talk_prog marriage~
	 if is_qmark($n) & forest_quest 301024
	 	if is_qmark($n) !& forest_quest 302048
	 		hop
	 		mpechoat $n $I ���G�z����Ӱѥ[��M�O�̦n���L���F�C
	 		mpechoat $n $I ���G�p�G�p�k�P�N�J���B§�|��A�ڤ@�w�ߨ�h�q���z�C
	 		mpechoat $n $I ���G���L�z���F�ڭ̤g�H�����o��h�����A�ڮɦb�L�H�^���C
	 		mpechoat $n $I ���G�ڳo���H����[1;36m�]�k���M(Magicla Steel Knife)[0m�N�e���z�a�I[0m
	 		mpoload 4215 36
	 		mpat 5 c enc knife
	 		mpmod_qmark $n forest_quest v2 + 2048
	 		give knife $n
	 		wave $n
	 	endif
	 endif
~
>talk_prog train~
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				bow $n
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n $I ���G��b�ӷP�±z�F�A�Ʊ�z��a�ۤp�l�X�h�����@���C
				mpechoat $n $I ���G �N�J �ڥ������D�~���@�ɪ����I�C
				mpforce hawk blush
				mpechoat $n $I ���G�H�� �N�J �԰��g�窺���ɡA�L�|�C�C�i�B�C
				mpechoat $n $I ���G[1;36m�� �N�J �u�����Ѿ԰����A�ѳ��Z�H�԰��ޥ�������ɡA[0m
				mpechoat $n $I ���G[1;36m�ڴN��k������L�C�� �N�J �ǳ�(ready)�n�A�N�i�H�X�o�C[0m
				mpforce alleina follow self
				mpforce alleina mpmod_qmark self forest_fiesta v0 = 8
				mpforce hawk mpmod_qmark self forest_fiesta v0 = 8
			endif
		endif
	endif
~
>talk_prog stop~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				mpechoaround $n $n �}�l�}�f�հ��L�̪��ɪ�....�C
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n [1;32m�A�V $I ����F�ۤv�@�N��U �N�J �������N�@�C[0m
				gasp $n
				mpechoat $n $I ���G��@�ӰV�m���Ѯv�O�۷��c���֤H���A
				mpechoat $n $I ���G�z�u���@�N�V�m(train)�o���������p�l�A���U�L�����H
			endif
		endif
	endif
~
>talk_prog start~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) & forest_quest 300128
		if insameroom(hawk)
			if insameroom(alleina)
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoaround $n [1;32m�L�̦n�����n�F�_��....�I[0m
				mpechoat $n $I ���G �N�J �A�Ӱ�����H���D�A�h�D $n �����H
				mpechoat $n $I ���G�A���D���A�ѧA���g�����M�����ܡH
				mpechoat $n $I ���G�����h�·Чڭ̤g�H���������H....�I
				mpforce alleina shake
				mpechoat $n �㽬�R(Alleina) ���G�����A�A�~�|�F�A�O�ڧ�Ӫ�....�C
				mpechoat $n [1;32m��L�̧n�����i�}��ɧA�߷Q�G�O�A�ۤv�ӥX�����ɭԤF(stop)�I[0m
			else
				mpechoat $n ����ǨӤ@�}�n�T�G�H�������I
			endif
		else
			mpechoat $n ����ǨӤ@�}�n�T�G�H�������I
		endif
	endif
~
>talk_prog quest~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 300063
		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		mpechoat $n $I ���G�A�w�g�����ڻݭn�A�����Ҧ�����....�C
		break
	endif
	if is_qmark($n) == forest_quest 100002
		mpechoat $n $I ���G�H�U�O�ڧƱ�A������������ȡG                                       
		if is_qmark($n) !& forest_quest 300002
			mpechoat $n $I ���G   �E[1;36m���o�h�A�D�Z�����(Scale of Snake Queen)[0m
		endif
		if is_qmark($n) !& forest_quest 300004
			mpechoat $n $I ���G   �E[1;36m���o��l����������(Eyes of Lion King)[0m
		endif
		if is_qmark($n) !& forest_quest 300008
			mpechoat $n $I ���G   �E[1;36m���o�}�ޥ��H�����ֽ�(Skin of Troll King)[0m
		endif
		if is_qmark($n) !& forest_quest 300016
			mpechoat $n $I ���G   �E[1;36m���o���줧�������(Burgeon of Old Tree)[0m
		endif
		if is_qmark($n) !& forest_quest 300032
			mpechoat $n $I ���G   �E[1;36m���o���������`����Ŧ(Heart of Fire Newt)[0m
		endif  
		mpechoat $n $I ���G�H�W���ǪF��A�i�H���U���v���X�i�H��ܤ��s���������~�C
		mpechoat $n $I ���G�ЧA�����H�W�����ȡA�H�ҩ��A�����e���������s�ɤڬ��q�C
	endif
~
>talk_prog restart~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) == forest_quest 200001
			if is_qmark($n) == forest_scale 100000
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n $I ���G�A�ұ���[1;36m�D�Z���[0m���Ȥw�g���s�}�l�F�C
				mpdel_qmark $n forest_scale
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_eyes 100000
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n $I ���G�A�ұ���[1;36m�������[0m���Ȥw�g���s�}�l�F�C
				mpdel_qmark $n forest_eyes
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_skin 100000
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n $I ���G�A�ұ���[1;36m���H���ֽ�[0m���Ȥw�g���s�}�l�F�C
				mpdel_qmark $n forest_skin
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) < forest_burgeon 100002
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n $I ���G�A�ұ���[1;36m������[0m���Ȥw�g���s�}�l�F�C
				mpdel_qmark $n forest_burgeon
				mpmod_qmark $n forest_quest v1 = 0
				break
			endif
			if is_qmark($n) == forest_heart 100000
				mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
				mpechoat $n $I ���G�A�ұ���[1;36m�����`����[0m���Ȥw�g���s�}�l�F�C
				mpdel_qmark $n forest_heart
				mpmod_qmark $n forest_quest v1 = 0
			endif
		endif
	endif
~	
>talk_prog assistant~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 100004
		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		mpmod_qmark $n forest_quest v0 + 1
		mpechoat $n $I ���G�S���D�A�ڥߨ�s�ڱڨ�ӾԤh�e�����U�A�I
		mpecho $I �j�ܤ@�n�A�ߨ�X�{��ӱj�����g�H�Ԥh�C
		mpechoat $n [1;32m�o��ӵ׭^�Ԥh�A�[�J�F�A��������C[0m
		mpechoaround $n [1;32m�o��ӵ׭^�Ԥh�A�[�J�F[0m $n [1;32m��������C[0m
		mpmload 4245
		mpforce native mpoload 4213
		mpforce native mpoload 4218
		mpforce native mpoload 4219
		mpforce native mpdef_rembch $n
		mpforce native mpat 5 wear all
		mpforce native mpgroup $n self 1 0
		mpmload 4245
		mpforce native mpoload 4213
		mpforce native mpoload 4220
		mpforce native mpoload 4223
		mpforce native mpdef_rembch $n
		mpforce native mpat 5 wear all
		mpforce native mpgroup $n self 1 0
		mpechoat $n $I ���G����e�����s�f�������i�c�� �ɤڬ��q �a�I
		wave $n
	endif
~		
>talk_prog p heart of fire newt~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_heart
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���b�i���L�����ȡC
			mpechoat $n $I ���G�бN�A���e�������ȵ������C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_heart 100002
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���O���b�i��o�����ȶܡH
			mpechoat $n $I ���G�p�G�A�Q�n���s�ӹL�A��ڻ� restart �C
			break
		endif
		if is_qmark($n) != forest_heart
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			nod $n
			mpechoat $n $I ���G����Q���������`�ͪ��b�Z����s���s�y�B�C
			mpechoat $n $I ���G���O�p�ߤ��s�����ŷ|�v�T�A���Ͳz���A�C
			mpechoat $n $I ���G�ӥB����]���}�ޥ��H���X�S�A���i�����C
			mpechoat $n $I ���G�A�����u�˦ۡv�����o�ӥ��ȡA�_�h�L�k�q�L�{�w�C
			wave $n
			mpadd_qmark $n forest_heart -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4248~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		give heart $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_heart 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 		clap $n
		 		mpechoat $n $I ���G���ߧA�������o[1;36m�����`����(Heart of Fire Newt)[0m�����ȡC
		 		mpmod_qmark $n forest_quest v2 + 32
		 		mpdel_qmark $n forest_heart
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk heart
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 			hop
		 			mpechoat $n $I ���G�ש󮳨줭���s�@[1;31m���K���](Orb of Flame)[0m����ƤF�C
		 			mpechoat $n $I ���G�ڲ{�b�ߨ�N�o�ǭ�Ʈ��h�����v�A�A�����H���ӡC
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n �A�ݨ� $I �X�������v��B����V���h�C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p burgeon of old tree~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_burgeon
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���b�i���L�����ȡC
			mpechoat $n $I ���G�бN�A���e�������ȵ������C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_burgeon 100003
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���O���b�i��o�����ȶܡH
			mpechoat $n $I ���G�p�G�A�Q�n���s�ӹL�A��ڻ� restart �C
			break
		endif
		if is_qmark($n) != forest_burgeon
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			nod $n
			mpechoat $n $I ���G���j�F��j�F�z�P�Z��쪺�����A���ӿ@�K���˪L�a�a�C
			mpechoat $n $I ���G�˪L�������A���@�ʥ��j������C
			mpechoat $n $I ���G�Ч⯫��y�ݪ���ޥ浹�ڡA���O�Фp�ߨ��̪����N�̡C
			mpechoat $n $I ���G�A�����u�˦ۡv�����o�ӥ��ȡA�_�h�L�k�q�L�{�w�C
			mpat 4245 mpoload 4247 floor
			wave $n
			mpadd_qmark $n forest_burgeon -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4247~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		give burgeon $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_burgeon 100002
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 		clap $n
		 		mpechoat $n $I ���G���ߧA�������o[1;36m���줧�������(Burgeon of Old Tree)[0m�����ȡC
		 		mpmod_qmark $n forest_quest v2 + 16
		 		mpdel_qmark $n forest_burgeon
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk burgeon
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 			hop
		 			mpechoat $n $I ���G�ש󮳨줭���s�@[1;31m���K���](Orb of Flame)[0m����ƤF�C
		 			mpechoat $n $I ���G�ڲ{�b�ߨ�N�o�ǭ�Ʈ��h�����v�A�A�����H���ӡC
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n �A�ݨ� $I �X�������v��B����V���h�C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p skin of troll king~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_skin
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���b�i���L�������ȡC
			mpechoat $n $I ���G�бN�A���e�������ȵ������C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_skin 100002
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���O���b�i��o�����ȶܡH
			mpechoat $n $I ���G�p�G�A�Q�n���s�ӹL�A��ڻ� restart �C
			break
		endif
		if is_qmark($n) != forest_skin
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			nod $n
			mpechoat $n $I ���G�}�ޥ��H���_�ަb�_���Z����s���s�}�U�C
			mpechoat $n $I ���G���F���H���Q�����j�����~�A��L�����H�]�|�����A�C
			mpechoat $n $I ���G�}�ޥ��H���O�q�D�`�D�`�j�A�Ʊ�A�d�U�p�ߡC
			mpechoat $n $I ���G�A�����u�˦ۡv�����o�ӥ��ȡA�_�h�L�k�q�L�{�w�C
			wave $n
			mpadd_qmark $n forest_skin -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4246~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		give skin $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_skin 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 		clap $n
		 		mpechoat $n $I ���G���ߧA�������o[1;36m�}�ޥ��H���ֽ�(Skin of Troll King)[0m�����ȡC
		 		mpmod_qmark $n forest_quest v2 + 8
		 		mpdel_qmark $n forest_skin
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk skin
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 			hop
		 			mpechoat $n $I ���G�ש󮳨줭���s�@[1;31m���K���](Orb of Flame)[0m����ƤF�C
		 			mpechoat $n $I ���G�ڲ{�b�ߨ�N�o�ǭ�Ʈ��h�����v�A�A�����H���ӡC
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n �A�ݨ� $I �X�������v��B����V���h�C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p eyes of lion king~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_eyes
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���b�i���L�����ȡC
			mpechoat $n $I ���G�бN�A���e�������ȵ������C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_eyes 100002
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���O���b�i��o�����ȶܡH
			mpechoat $n $I ���G�p�G�A�Q�n���s�ӹL�A��ڻ� restart �C
			break
		endif
		if is_qmark($n) != forest_eyes
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			nod $n
			mpechoat $n $I ���G��l�̪����b�Z��쪺�F�n��C
			mpechoat $n $I ���G���F����Q�����j�����~�A��L����l�]�|�����A�C
			mpechoat $n $I ���G�p�S���۷�ⴤ�A�ХR��ۤv����O����A�h�������ȡC
			mpechoat $n
			mpechoat $n $I ���G�A�����u�˦ۡv�����o�ӥ��ȡA�_�h�L�k�q�L�{�w�C
			wave $n
			mpadd_qmark $n forest_eyes -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4245~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		give eyes $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_eyes 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 		clap $n
		 		mpechoat $n $I ���G���ߧA�������o[1;36m�������(Eyes of Lion King)[0m�����ȡC
		 		mpmod_qmark $n forest_quest v2 + 4
		 		mpdel_qmark $n forest_eyes
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk eyes
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 			hop
		 			mpechoat $n $I ���G�ש󮳨줭���s�@[1;31m���K���](Orb of Flame)[0m����ƤF�C
		 			mpechoat $n $I ���G�ڲ{�b�ߨ�N�o�ǭ�Ʈ��h�����v�A�A�����H���ӡC
		 			mpmod_qmark $n forest_quest v0 + 1
		 			op e
		 			mpechoat $n �A�ݨ� $I �X�������v��B����V���h�C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>talk_prog p scale of snake queen~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		break
	endif
	if is_qmark($n) == forest_quest 200001
		if is_qmark($n) != forest_scale
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���b�i���L�����ȡC
			mpechoat $n $I ���G�бN�A���e�������ȵ������C
		endif
		break
	endif
	if is_qmark($n) == forest_quest 100002
		if is_qmark($n) < forest_scale 100002
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			mpechoat $n $I ���G�A�{�b���O���b�i��o�����ȶܡH
			mpechoat $n $I ���G�p�G�A�Q�n���s�ӹL�A��ڻ� restart �C
			break
		endif
		if is_qmark($n) != forest_scale
			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
			nod $n
			mpechoat $n $I ���G�D�Z�b�Z��쪺�F��h�A�Ϫ��̸̭��C
			mpechoat $n $I ���G��Ӫh�A�Ϫ��h��P�I�k�ʪ��h�A���|�ˮ`�A���ֽ��C
			mpechoat $n $I ���G�]���Цh�[�p�ߡC
			mpechoat $n $I ���G�A�����u�˦ۡv�����o�ӥ��ȡA�_�h�L�k�q�L�{�w�C
			wave $n
			mpadd_qmark $n forest_scale -1 0 0 0 0 1
			mpmod_qmark $n forest_quest v1 = 1
		endif
	endif
~
>give_prog 4244~
	if is_qmark($i) == forest_fiesta 300000
		mpechoat $n $I ���G��p�A�{�b���O�צ~�������n��l�C
		mpechoat $n $I ���G������ƽЫ��צ~����������A���C
		give scale $n
		break
	endif
	if is_qmark($n) == forest_quest 100002
		 if is_qmark($n) == forest_scale 100001
		 	if is_qmark($n) == forest_quest 200001
		 		mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 		clap $n
		 		mpechoat $n $I ���G���ߧA�������o[1;36m�D�Z���(Scale of Snake Queen)[0m�����ȡC
		 		mpmod_qmark $n forest_quest v2 + 2
		 		mpdel_qmark $n forest_scale
		 		mpmod_qmark $n forest_quest v1 = 0
		 		mpjunk scale
		 		if is_qmark($n) == forest_quest 300062
		 			mpechoaround $n $n ���μ��m���g�y�P $I ����....�C
		 			hop
		 			mpechoat $n $I ���G�ש󮳨줭���s�@[1;31m���K���](Orb of Flame)[0m����ƤF�C
		 			mpechoat $n $I ���G�ڲ{�b�ߨ�N�o�ǭ�Ʈ��h�����v�A�A�����H���ӡC
		 			mpmod_qmark $n forest_quest v0 + 1
		 			open e
		 			close e
		 			mpecho �A�ݨ� $I �X�������v��B����V���h�C
		 			mpgoto 4304
		 		endif
		 	endif
		 endif
	endif
~
>fight_prog 60~
	if insameroom(alleina)
		mpgroup self alleina 1 0
		protect alleina
		break
	endif
	if isaffected($i) !& 128
		mpat 5 c sanctuary
		break
	endif
	if isaffected($i) & 1
		mpat c 'cure blindness'
	endif
~
>fight_prog 40~
	mpat 5 c heal
	wear sword
	wear sword
	disarm
	if rand(30)
		coh
		break
	endif
	if rand(30)
		frenzy
		break
	endif
	if rand(10)
		mpat 5 invoke
	else
		c blind
		c heal
		hunt $r
	endif
~
>death_prog 50~
	mpdis_magic self 1
	mpat 5 c inf
	if is_qmark($n) != forest_enemy
		mpadd_qmark $n forest_enemy 50 0 0 0 0 1
	else
		mpmod_qmark $n forest_enemy timer + 50
	endif
~
>tick_prog 100~
	if is_qmark($i) == forest_fiesta 300000
		mpmod_qmark self forest_fiesta v1 + 1
		if is_qmark($i) == forest_fiesta 200012
			mpmod_qmark self forest_fiesta v0 = 1
			mpforce alleina mpmod_qmark self forest_fiesta v0 + 1			break
		endif
		if is_qmark($i) == forest_fiesta 200024
			mpmod_qmark self forest_fiesta v0 = 2
			mpforce alleina mpmod_qmark self forest_fiesta v0 + 1
			mpecho $I [1;32m���M�j�ܤ@�n�A���ۦb�s���������F�U��....�C[0m
			mpecho $I [1;32m���_���V�s�����������������ť�������ܡC[0m
			break
		endif
		if is_qmark($i) > forest_fiesta 200024
			if is_qmark($i) < forest_fiesta 200048
				if rand(30)
					mpecho $I [1;32m���_���V�s�������������ǧAť�������ܡC[0m
					break
				endif
				if rand(30)
					mpecho ����(Thorba) [1;32m�b�a�W�C�C���汵�񯫹��A�n���N����S��N�q�C[0m
					break
				endif
				if rand(30)
					mpecho ����(Thorba) [1;32m�}�l���@�ǩG�y�A�n���O��D���w���C[0m
				endif
			endif
			break
		endif
		if is_qmark($i) == forest_fiesta 200048
			mpecho $I [1;32m���G���~��V�s�����������ѫ��F�C[0m
			mpecho $I [1;32m�_��������ë�����A�צ~�������x��^�S�A�׶}�l�F�C[0m
			break
		endif
		if is_qmark($i) == forest_fiesta 200072
			mpdel_qmark self forest_fiesta
			rumor [1;32m�ڻD�Z��˪L�g�H�צ~���w�g�����F�C[0m
			mptransfer thorba 4304
			mpgoto 4203
			mppurge feetsteps
			mpgoto 4206
			mppurge feetsteps
			mpgoto 4209
			mppurge feetsteps
			mpgoto 4224
			if insameroom(hawk)
				mppurge hawk
			endif
			if insameroom(alleina)
				mpforce alleina mpdel_qmark self forest_fiesta
				mptransfer alleina 4296
			endif
			mpgoto 4296
		endif
		break
	endif
	if month($) == 0
	or month($) == 6
	or month($) == 12
		if day($) == 1
			if hour($) == 6
				rumor [1;32m�ڻD���Ӥ�@�ת��Z��˪L�g�H�צ~���Y�N�}�l�C[0m
				endif
			break
			endif
		endif
		if day($) == 18
			if is_qmark($i) != forest_fiesta
				rumor [1;32m�ڻD�Z��˪L�g�H�צ~���w�g�}�l�F�C[0m
				mpat 4304 mpforce thorba mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpforce alleina mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpforce alleina mpgoto 4297
				mpat 4304 mpforce thorba mpgoto 4297
				mpgoto 4297
				mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
				mpmload 4243
				mpforce native-a mpadd_qmark self forest_fiesta -1 0 0 0 0 1
			endif
			break
		endif
	endif
	if hour($) > 5
		if hour($) < 7
			wak
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4305
					mpecho $I �_�ɦ��F���i�y�A���G�n�h�����F�C
					open e
					e
					close w
					e
					e
				else
					mpgoto 4305
					mpecho $I �_�ɦ��F���i�y�A���G�n�h�����F�C
					open e
					e
					close w
					e
					e
				endif
			endif
		endif
		break
	endif
	if hour($) > 12
		if hour($) < 14
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4296
					mpecho $I �I�F�I�Y�A�M�᩹�ۤv����B���h�C
					w
					w
					open w
					w
					close e
					rest
				else
					mpgoto 4296
					mpecho $I �I�F�I�Y�A�M�᩹�ۤv����B���h�C
					w
					w
					open w
					w
					close e
					rest
				endif
			endif
		endif
		break
	endif
	if hour($) > 0
		if hour($) < 2
			if is_qmark($i) != forest_fiesta
				if inroom($i) == 4305
					mpecho $I ���F�@�ӫ���A�M����Y�K�ΡC
					sleep
				else
					mpgoto 4305
					mpecho $I ���F�@�ӫ���A�M����Y�K�ΡC
					sleep
				endif
			endif
		endif
	endif
~
|