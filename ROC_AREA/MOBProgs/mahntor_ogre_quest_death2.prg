>death_prog 100~
if is_qmark($n) <= mahntor_ogre_quest1 300049
        mpmod_qmark $n mahntor_ogre_quest1 v2 + 3
        mpechoat $n �Ҧ���������A����N�������ɡI
        break
else
	mpcall mahntor_ogre_quest_death_call $n
endif
~
|