>rand_prog 20~
say 幹！去！呸！
~
>greet_prog 10~
if level($n) < 5
or level($n) >= 15
	break
endif
mpkill $n
~
>death_prog 60~
mpechoaround $i $I 把頭往後一甩，瘋狂的叫了一聲。
~
|
