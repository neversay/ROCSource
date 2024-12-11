>talk_prog topic~
       say 賞點錢吧，大爺...
~
>greet_prog 70~
if ispc($n)
        beg $n
        say 大爺，施捨一點吧？
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
	mpechoat $n $I 說 '您的好心會有好報的...'
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
say 救命！誰來救我啊？
if rand(50)
        say Ouch!
else
        say 我在吐血了！
endif
~
>death_prog 50~
if rand(50)
        mpechoaround $i $I says '我現在要到一個更好的地方了！'
else
        mpechoaround $i $I says '上帝原諒我，因為我是罪人....'
endif
~
|
