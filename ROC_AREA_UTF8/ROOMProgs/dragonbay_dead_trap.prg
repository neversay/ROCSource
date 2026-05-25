>room_enter 127 100~
echoat $n [1;36m你發覺你全身都無法動彈了！[0m
add_affect $n 'paralyze' 1130 100 39 1000 268435456
echoat $n [1;33m你全身都感到相當的不舒服！[0m
echoaround $n [1;36m你看到 $N [1;36m也跌進了這個要命的蜘蛛網！[0m
add_affect $n 'disease' 1130 100 1 -10 1099511627776
add_affect $n 'disease' 1130 100 2 -10 1099511627776
add_affect $n 'disease' 1130 100 3 -10 1099511627776
add_affect $n 'disease' 1130 100 4 -10 1099511627776
add_affect $n 'disease' 1130 100 5 -10 1099511627776
add_affect $n 'disease' 1130 100 41 -5000 1099511627776
add_affect $n 'disease' 1130 100 42 -5000 1099511627776
add_affect $n 'disease' 1130 100 17 1000 1099511627776
~
>room_defun dragonbay_dead_trap_fun~
echo [1;35m你感到你全身的力量都正在流失！全身都感到相當的不舒服！[0m
modhmv pc 03.10 15 0
modhmv pc 01.200 200 0
modhmv pc 13.10 15 0
modhmv pc 11.200 200 0
modhmv pc 23.10 15 0
modhmv pc 21.200 200 0
~
|
