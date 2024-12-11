>talk_prog topic~
	say 城門晚上會關起來，以保護市民安全。所以請注意
	say 自己出入的時間。(輸入time可以知道現在的時間)
~
>greet_prog 75~
if insameroom($n)
	if isgood($n)
		if level($n) > 18
			mpechoat $n $I看著你走過來。
			mpechoaround $n $I注意到$N走過來。
		endif
	endif
	if isevil($n)
        	if rand(30)
                	mpechoat $n $I警告你快點離開這裡.
	                mpechoaround $n $I警告$N快點離開.
        	        say 這裡不歡迎邪惡的人！
	        endif
	endif
endif
~
>death_prog 60~
yell 永遠的史卡拉貝！！！
~
|
