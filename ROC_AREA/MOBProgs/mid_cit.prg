>entry_prog 30~
emote 到處尋找嫌犯。
~
>greet_prog 40~
look $n
if isgood($n)
        say 你好。
        smile $n
else
	hehe $n
endif
~
>rand_prog 20~
if rand(50)
        emote 哼著小調兒。
else
        sing
endif
~
>speech_prog p 我的女朋友離開我了~
comfort $n 
~
>death_prog 60~
yell 光明不滅，聖城永存！！！
~
|