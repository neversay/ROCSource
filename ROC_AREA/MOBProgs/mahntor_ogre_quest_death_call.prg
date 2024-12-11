>defun_prog mahntor_ogre_quest_death_call~
if is_qmark($n) >= mahntor_ogre_quest1 300050
        mpechoat $n 因為殺人無數！曼托爾城堡全面對$n發出通緝！」
        mpechoat $n 遠方的巨魔酋長對你用心電感應：「您做的太好了！請回?        mpechoat $n 回來順便告訴我一聲呀！(Finish)
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
	say 還我族人命來！
	mpkill $n
endif
~
|