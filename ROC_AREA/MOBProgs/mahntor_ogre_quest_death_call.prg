>defun_prog mahntor_ogre_quest_death_call~
if is_qmark($n) >= mahntor_ogre_quest1 300050
        mpechoat $n �]�����H�L�ơI�Ҧ�������������$n�o�X�q�r�I�v
        mpechoat $n ���誺���]������A�Τ߹q�P���G�u�z�����Ӧn�F�I�Ц^?        mpechoat $n �^�Ӷ��K�i�D�ڤ@�n�r�I(Finish)
        if is_qmark($n) == mahntor_ogre_quest1 100002
                mpmod_qmark $n mahntor_ogre_quest1 v0 = 3
        endif
        if is_qmark($n) == mahntor_ogre_quest1 200002
                mpmod_qmark $n mahntor_ogre_quest1 v1 = 3
        endif
endif
~
>entry_greet_prog 100~
if is_qmark($n) >= mahntor_ogre_quest1 300050
	say �٧ڱڤH�R�ӡI
	mpkill $n
endif
~
|