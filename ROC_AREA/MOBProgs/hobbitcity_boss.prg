>death_prog 100~
if ispc($n)
    yell $N�O�@�ӱ��H����A�j�a�n���ڳ����z�I
    mpdis_magic self all
    mpat 5 c infr
    mpadd_qmark $n hobbitcity_killer 50 1 0 0 0 1
endif
~
>fight_prog 20~
if mobcount(14006) < 10
yell �ӤH�ڡI$N�b�o�̳x���ڡI
mpecho [36m�@��v�wĵ��(security guard)ť��ۥs�榣���ӡI[0m
mpmload 14006
mpforce security protect $i
endif
~
>greet_prog 50~
if is_qmark($n) == hobbitcity_killer
yell ĵ�ç֨ӡI���H��$N�b�o�̡I
mpecho [36m�@��v�wĵ��(security guard)�^�^�����]�ӡI[0m
mpmload 14006
mpforce security say �n�p�l�A�٤�����N��C
mpforce security mpkill $n
endif
~
|