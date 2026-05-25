>death_prog 100~
if ispc($n)
    yell $N是一個殺人兇手，大家要為我報仇哇！
    mpdis_magic self all
    mpat 5 c infr
    mpadd_qmark $n hobbitcity_killer 50 1 0 0 0 1
endif
~
>fight_prog 20~
if mobcount(14006) < 10
yell 來人啊！$N在這裡逞兇啊！
mpecho [36m一位治安警衛(security guard)聽到喊叫急忙趕來！[0m
mpmload 14006
mpforce security protect $i
endif
~
>greet_prog 50~
if is_qmark($n) == hobbitcity_killer
yell 警衛快來！殺人犯$N在這裡！
mpecho [36m一位治安警衛(security guard)匆匆忙忙跑來！[0m
mpmload 14006
mpforce security say 好小子，還不束手就擒。
mpforce security mpkill $n
endif
~
|