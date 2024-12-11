>room_command probe south~
if isfight($n)
    echoat $n [1;36m§A¥¿¦b¦£µÛ¾Ô°«©O¡I¨º¨Óªº®É¶¡±´¯Á§r¡I[0m
else
echoat $n §A¶}©l±´¯ÁµÛ«n¤èªº¥Û¾À¡C
    if wis($n) > 23
        callout 1 rcall holytown_rockseal_check_ending3 $n
    else
        callout 1 echoat $n [1;36m¦b§A¥J²Ó±´¯Á¤§«á¡AÁÙ¬O¤£¾å±o«n¤èªº¥Û¾À¨s³º¦³¨S¦³²§ª¬¡C[0m
        callout 1 modhmv $n 33.5 10 0
    endif
endif
~
|
