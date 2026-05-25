>room_defun holytown_rockseal_check_ending2~
if roomvalue($0) == 2
    echoat $n [1;36m以你聰穎的智慧探索後的結果告訴你，東方石壁是有機會可以通行的。[0m
else
    echoat $n [1;36m以你聰穎的智慧探索後的結果告訴你，東方石壁是無法通行的。[0m
endif
modhmv $n 33.5 10 0
~
|
