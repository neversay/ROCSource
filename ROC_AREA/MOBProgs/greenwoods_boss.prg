>death_prog 100~
if level($n) > 36
if ispc($n)
    yell $N是奴役綠樹王國的壞人，大家要為我報仇哇！
    mpdis_magic self all
    mpat 5 c infr
    mpadd_qmark $n greenwoods_killer 20 1 0 0 0 1
endif
endif
~
|