>talk_prog topic~
	say �����ߤW�|���_�ӡA�H�O�@�����w���C�ҥH�Ъ`�N
	say �ۤv�X�J���ɶ��C(��Jtime�i�H���D�{�b���ɶ�)
~
>greet_prog 75~
if insameroom($n)
	if isgood($n)
		if level($n) > 18
			mpechoat $n $I�ݵۧA���L�ӡC
			mpechoaround $n $I�`�N��$N���L�ӡC
		endif
	endif
	if isevil($n)
        	if rand(30)
                	mpechoat $n $Iĵ�i�A���I���}�o��.
	                mpechoaround $n $Iĵ�i$N���I���}.
        	        say �o�̤��w�器�c���H�I
	        endif
	endif
endif
~
>death_prog 60~
yell �û����v�d�Ԩ��I�I�I
~
|
