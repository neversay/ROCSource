>talk_prog topic~
       say ���I���a�A�j��...
~
>greet_prog 70~
if ispc($n)
        beg $n
        say �j�ݡA�I�ˤ@�I�a�H
endif
~
>bribe_prog 10000~
dance $n
french $n
~
>bribe_prog 1000~
say Oh my GOD!  Thank you! Thank you!
french $n
if is_qmark($i) == ultima_beggar
	break
endif
if is_qmark($n) == ultima_britain
	mpechoat $n $I �� '�z���n�߷|���n����...'
	mpmod_qmark $n ultima_britain v1 + 1
	mpadd_qmark self ultima_beggar 100 0 0 0 1 0
	mpjunk all.coins
endif
~
>bribe_prog 100~
say Wow!  Thank you! Thank you!
~
>bribe_prog 1~
thank $n
~
>fight_prog 20~
say �ϩR�I�֨ӱϧڰڡH
if rand(50)
        say Ouch!
else
        say �ڦb�R��F�I
endif
~
>death_prog 50~
if rand(50)
        mpechoaround $i $I says '�ڲ{�b�n��@�ӧ�n���a��F�I'
else
        mpechoaround $i $I says '�W�ҭ�̧ڡA�]���ڬO�o�H....'
endif
~
|
