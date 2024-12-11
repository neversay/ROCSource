>room_defun quest_list~
at $b echoat $b [1;37m§A¥i¥H¬d¬Ýªº¥ô°È°Ï°ì:[0m
at $b echoat $b ²Ä¹s­¶ [1;33m»ÈÀs®üÆWº[¦a¼hÅ]°ì             [37m(      00       )[0m
at $b echoat $b ²Ä¤@­¶ [1;32m¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì I           [37m(      01       )[0m
at $b echoat $b ²Ä¤G­¶ [1;32m¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì II          [37m(      02       )[0m
at $b echoat $b ²Ä¤T­¶ [1;32m¥v¥d©Ô¨©¦èÃä°Ï°ì I             [37m(      03       )[0m
at $b echoat $b ²Ä¥|­¶ [1;32m¥v¥d©Ô¨©¦èÃä°Ï°ì II            [37m(      04       )[0m
at $b echoat $b ²Ä¤­­¶ [1;32m¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï I   [37m(      05       )[0m
at $b echoat $b ²Ä¤»­¶ [1;32m¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï II  [37m(      06       )[0m
at $b echoat $b ²Ä¤C­¶ [1;32m¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï I         [37m(      07       )[0m
at $b echoat $b ²Ä¤K­¶ [1;32m¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï II        [37m(      08       )[0m
at $b echoat $b ²Ä¤E­¶ [1;36m°gÃú¤§®q¥D°Ï                   [37m(      09       )[0m
at $b echoat $b ²Ä¤Q­¶ [1;36m°gÃú¤§®q©`¹Ï¤ý°ê               [37m(      10       )[0m
at $b echoat $b ²Ä11­¶ [1;36m°gÃú¤§®q¤§¨ä¥L°Ï°ì I           [37m(      11       )[0m
at $b echoat $b ²Ä12­¶ [1;36m°gÃú¤§®q¤§¨ä¥L°Ï°ì II          [37m(      12       )[0m
at $b echoat $b ²Ä13­¶ [1;36m°gÃú¤§®q¤§¨ä¥L°Ï°ì III         [37m(      13       )[0m
def_rembch null-tar
~
>room_defun q00~
at $b echoat $b .             [1;36m[»ÈÀs®üÆWº[¦a¼hÅ]°ì][0m
at $b echoat $b .             [1;37m{»ÈÀs®üÆW}[0m
if is_qmark($b) == dragonbay_mayor_secret 100010
or is_qmark($b) & dragonbay_quest_mark 100004
        at $b echoat $b [1;35m¯«¿ï¤§¤H                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¯«¿ï¤§¤H                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{Å]²´ÂOªL}[0m
if is_qmark($b) & dragonbay_quest_mark 200001
        at $b echoat $b [1;35m¤jªø¦Ñªº§Æ±æ                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤jªø¦Ñªº§Æ±æ                 ¥¼§¹[0m
endif
if is_qmark($b) & dragonbay_quest_mark 200002
        at $b echoat $b [1;35m¿U¿N                         §¹¦¨[0m
else
        at $b echoat $b [1;32m¿U¿N                         ¥¼§¹[0m
endif
if is_qmark($b) & dragonbay_quest_mark 200004
        at $b echoat $b [1;35m¸ê·½¦^¦¬                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¸ê·½¦^¦¬                     ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q01~
at $b echoat $b .             [1;36m[¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì I][0m
at $b echoat $b .             [1;37m{¥v¥d©Ô¨©«°}[0m
if is_qmark($b) == skara_baby_quest1 100005
        at $b echoat $b [1;35m¦ÑªÌªº°U¥I                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¦ÑªÌªº°U¥I                   ¥¼§¹[0m
endif
if is_qmark($b) == skara_baby_quest2 100003
        at $b echoat $b [1;35mÂåªÌªºÀs¸ª                   §¹¦¨[0m
else
        at $b echoat $b [1;32mÂåªÌªºÀs¸ª                   ¥¼§¹[0m
endif
if is_qmark($b) == skara_baby_quest3 100003
        at $b echoat $b [1;35m¹Î¶¤»P¦X§@                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¹Î¶¤»P¦X§@                   ¥¼§¹[0m
endif
if is_qmark($b) == midennir_lostboy_exp
        at $b echoat $b [1;35mµJ¼{ªº¥À¿Ë                   §¹¦¨[0m
else
        at $b echoat $b [1;32mµJ¼{ªº¥À¿Ë                   ¥¼§¹[0m
endif
if primeattr($b) == 2
or class($b) == 2
or class($b) == 9
or class($b) == 10
or class($b) == 17
        at $b echoat $b [1;30m(¯µ)[37mµs¸é¤½·|¥ô°È:[0m
if is_qmark($b) !& thief_quest_list 100001
        at $b echoat $b [1;35m--¯µ±K½Õ¬d                   §¹¦¨[0m
else
        at $b echoat $b [1;32m--¯µ±K½Õ¬d                   ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100002
        at $b echoat $b [1;35m--§®¤âºëºë¨à                 §¹¦¨[0m
else
        at $b echoat $b [1;32m--§®¤âºëºë¨à                 ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100008
        at $b echoat $b [1;35m--§®¤âªÅªÅ¨à                 §¹¦¨[0m
else
        at $b echoat $b [1;32m--§®¤âªÅªÅ¨à                 ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100004
        at $b echoat $b [1;35m--·t³q±¡´Ú                   §¹¦¨[0m
else
        at $b echoat $b [1;32m--·t³q±¡´Ú                   ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100032
        at $b echoat $b [1;35m--§Tº»                       §¹¦¨[0m
else
        at $b echoat $b [1;32m--§Tº»                       ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100016
        at $b echoat $b [1;35m--·t±þ                       §¹¦¨[0m
else
        at $b echoat $b [1;32m--·t±þ                       ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100064
        at $b echoat $b [1;35m--¦å¸{·t±þ                   §¹¦¨[0m
else
        at $b echoat $b [1;32m--¦å¸{·t±þ                   ¥¼§¹[0m
endif
if is_qmark($b) !& thief_quest_list 100128
        at $b echoat $b [1;35m--ªÅ¤â¤J¥Õ¤b                 §¹¦¨[0m
else
        at $b echoat $b [1;32m--ªÅ¤â¤J¥Õ¤b                 ¥¼§¹[0m
endif
if is_qmark($b) == thief_quest_reward 100001
        at $b echoat $b [1;35m--µLµ¥¯Å·t¾¹¾c               §¹¦¨[0m
else
        at $b echoat $b [1;32m--µLµ¥¯Å·t¾¹¾c               ¥¼§¹[0m
endif
endif
if primeattr($b) == 3
or class($b) == 0
or class($b) == 5
or class($b) == 6
or class($b) == 15
        at $b echoat $b [1;30m(¯µ)[37mªk®v¤½·|¥ô°È:[0m
if is_qmark($b) == mage_envoy_quest 100024
        at $b echoat $b [1;35m--¥úºa±K¨Ï                   §¹¦¨[0m
else
        at $b echoat $b [1;32m--¥úºa±K¨Ï                   ¥¼§¹[0m
endif
endif
if is_qmark($b) == enable_level_40
        at $b echoat $b [1;35m«iªÌºÙ¸¹                     §¹¦¨[0m
else
        at $b echoat $b [1;32m«iªÌºÙ¸¹                     ¥¼§¹[0m
endif
if is_qmark($b) == enable_level_41
        at $b echoat $b [1;35m­^¶¯ºÙ¸¹                     §¹¦¨[0m
else
        at $b echoat $b [1;32m­^¶¯ºÙ¸¹                     ¥¼§¹[0m
endif
if is_qmark($b) == skarabrae_joe_quest 100004
        at $b echoat $b [1;35m³ìÂÅ¦hªº·Ð´o                 §¹¦¨[0m
else
        at $b echoat $b [1;32m³ìÂÅ¦hªº·Ð´o                 ¥¼§¹[0m
endif
if is_qmark($b) == commerce_crystal_quest 100006
        at $b echoat $b [1;35m¯]Ä_ÀË©w                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¯]Ä_ÀË©w                     ¥¼§¹[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100001
        at $b echoat $b [1;35m¾x°­ªº®Ñ¥»                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¾x°­ªº®Ñ¥»                   ¥¼§¹[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100002
        at $b echoat $b [1;35m¦UÃh°­­Lªº©e°U               §¹¦¨[0m
else
        at $b echoat $b [1;32m¦UÃh°­­Lªº©e°U               ¥¼§¹[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100004
        at $b echoat $b [1;35m«O±K¨¾¿Ò                     §¹¦¨[0m
else
        at $b echoat $b [1;32m«O±K¨¾¿Ò                     ¥¼§¹[0m
endif
if is_qmark($b) == valentine_quest 300511
or is_qmark($b) == valentine_quest 301023
        at $b echoat $b [1;35m·RªºÂ§ª«                     §¹¦¨[0m
else
        at $b echoat $b [1;32m·RªºÂ§ª«                     ¥¼§¹[0m
endif
if is_qmark($b) != commerce_enemy
at $b echoat $b .             [1;37m{¥v¥d©Ô¨©°Ó·|}[0m
if is_qmark($b) == adv_guild_level
	at $b echoat $b [1;30m«_ÀIªÌ¤½·|µû»ù:[0m
	if is_qmark($b) & adv_guild_level 300004
		at $b echoat $b [1;35m--§Nªù©e°U·R¦nªÌ             §¹¦¨[0m
	else
		if is_qmark($b) & adv_guild_level 300002
			at $b echoat $b [1;35m--Âø¨Æ¯à¤â                   §¹¦¨[0m
		else
			if is_qmark($b) & adv_guild_level 300001
				at $b echoat $b [1;35m--¤½·|·s°ÑªÌ                 §¹¦¨[0m
			endif
		endif
	endif
endif
if is_qmark($b) == commerce_noob_mage 300001
or is_qmark($b) == commerce_noob_mage 300002
or is_qmark($b) & commerce_subquest 100001
or is_qmark($b) & commerce_subquest 200001
		at $b echoat $b [1;35mªì¥X­TÃfªºªk®v               §¹¦¨[0m
else
		at $b echoat $b [1;32mªì¥X­TÃfªºªk®v               ¥¼§¹[0m
endif
if is_qmark($b) == scar_adv_helping 300001
or is_qmark($b) == scar_adv_helping 300002
or is_qmark($b) & commerce_subquest 100002
or is_qmark($b) & commerce_subquest 200002
		at $b echoat $b [1;35m«_ÀIªÌªº¤¯¸q                 §¹¦¨[0m
else
		at $b echoat $b [1;32m«_ÀIªÌªº¤¯¸q                 ¥¼§¹[0m
endif
if is_qmark($b) & commerce_subquest 100004
		at $b echoat $b [1;35m¦a¤U¥æ©öÆU                   §¹¦¨[0m
endif
if is_qmark($b) & commerce_subquest 100008
		at $b echoat $b [1;35m°Ò¥ýªº¯ª²£                   §¹¦¨[0m
endif
if is_qmark($b) == pharmacist_level
	if is_qmark($b) & pharmacist_level 100004
		at $b echoat $b [1;30m(¯µ)[0m[1;35mÃÄ«~Áå¦¨ I               §¹¦¨[0m
	else
		at $b echoat $b [1;30m(¯µ)[0m[1;32mÃÄ«~Áå¦¨ I               ¥¼§¹[0m
	endif
endif
endif
def_rembch null-tar
~
>room_defun q02~
at $b echoat $b .             [1;36m[¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì II][0m
at $b echoat $b .             [1;37m{¤pºëÆF§ø}[0m
if is_qmark($b) == smurf_save 100002
        at $b echoat $b [1;35m´À¤pºëÆF¥X¤f®ð               §¹¦¨[0m
else
        at $b echoat $b [1;32m´À¤pºëÆF¥X¤f®ð               ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{°¨À¸¹Î}[0m
if is_qmark($b) & circus_quest 100001
        at $b echoat $b [1;35m¸Ñ±Ï¤u§@¤H­û                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¸Ñ±Ï¤u§@¤H­û                 ¥¼§¹[0m
endif
if is_qmark($b) & circus_quest 100002
        at $b echoat $b [1;35m¤pÀj¤ñªº¶ý¶ý                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤pÀj¤ñªº¶ý¶ý                 ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{±Kµn¥§´ËªL}[0m
if is_qmark($b) == midennir_bard_quest 100002
        at $b echoat $b [1;35m§u¹C¸Ö¤H¤§ºq                 §¹¦¨[0m
else
        at $b echoat $b [1;32m§u¹C¸Ö¤H¤§ºq                 ¥¼§¹[0m
endif
if is_qmark($b) >= midennir_quest 100001
        at $b echoat $b [1;35mª½ìê­ô¥¬ªLÁ`³¡               §¹¦¨[0m
else
        at $b echoat $b [1;32mª½ìê­ô¥¬ªLÁ`³¡               ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{±_¥Þ³h¥Á°Ï}[0m
if is_qmark($b) & slum_libei_quest 100001
        at $b echoat $b [1;35m¥´¹s¤u                       §¹¦¨[0m
else
        at $b echoat $b [1;32m¥´¹s¤u                       ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¹«¤H±_¥Þ}[0m
if is_qmark($b) == wererat_save 100002
        at $b echoat $b [1;35m¥i¼¦ªº¥}¥Ç                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¥i¼¦ªº¥}¥Ç                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¥_¤è¥­­ì}[0m
if is_qmark($b) == plains_slaver_quest 200004
        at $b echoat $b [1;35m¥£Áõ³c¤l                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¥£Áõ³c¤l                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{±Ð°ó¹Óµ¢}[0m
if is_qmark($b) == chapel_question 100004
        at $b echoat $b [1;35m¹Ó¶é¤§Á¼                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¹Ó¶é¤§Á¼                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¨¸Às²½¾Â}[0m
if is_qmark($b) == cult_question_end
        at $b echoat $b [1;35m¬@±Ï®L¬¥¸¦                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¬@±Ï®L¬¥¸¦                   ¥¼§¹[0m
endif
if is_qmark($b) == cult_rock_end
        at $b echoat $b [1;35m¤j¦a¤§¥Û                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¤j¦a¤§¥Û                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¦aºë§ø}[0m
if is_qmark($b) & gnome_quest 100002
        at $b echoat $b [1;35m±Úªøªº·Ð´o                   §¹¦¨[0m
else
        at $b echoat $b [1;32m±Úªøªº·Ð´o                   ¥¼§¹[0m
endif
if is_qmark($b) & gnome_quest 100004
        at $b echoat $b [1;35m¤j´c°­¯Éª§                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¤j´c°­¯Éª§                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{«q¶Ã­x«°¹ë}[0m
if is_qmark($b) > rebel_questmark 100004
        at $b echoat $b [1;35m±Ù¤F¨º­Óª±«ÍÅéªº³Ã¥ë¡I       §¹¦¨[0m
else
        at $b echoat $b [1;32m±Ù¤F¨º­Óª±«ÍÅéªº³Ã¥ë¡I       ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100017
        at $b echoat $b [1;35m´N¦a¨ú§÷                     §¹¦¨[0m
else
        at $b echoat $b [1;32m´N¦a¨ú§÷                     ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100021
        at $b echoat $b [1;35m´N¦a¨ú§÷¡E¨ä¤§¤G             §¹¦¨[0m
else
        at $b echoat $b [1;32m´N¦a¨ú§÷¡E¨ä¤§¤G             ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100029
        at $b echoat $b [1;35m·m¿ú¡B·mÂ³¡B·m......         §¹¦¨[0m
else
        at $b echoat $b [1;32m·m¿ú¡B·mÂ³¡B·m......         ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100034
        at $b echoat $b [1;35m¿U¿N§a¡A°«§Ó                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¿U¿N§a¡A°«§Ó                 ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100038
        at $b echoat $b [1;35m¬µ¼uÅ]Ä±¿ô¤§¤é               §¹¦¨[0m
else
        at $b echoat $b [1;32m¬µ¼uÅ]Ä±¿ô¤§¤é               ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100042
        at $b echoat $b [1;35m¯}Ãa®É¨è                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¯}Ãa®É¨è                     ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100048
        at $b echoat $b [1;35m¾a¼L¤ÚÁÙ¬O¾a®±ÀY¡H           §¹¦¨[0m
else
        at $b echoat $b [1;32m¾a¼L¤ÚÁÙ¬O¾a®±ÀY¡H           ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100055
        at $b echoat $b [1;35m³Ì«áªº©è§Ü                   §¹¦¨[0m
else
        at $b echoat $b [1;32m³Ì«áªº©è§Ü                   ¥¼§¹[0m
endif
if is_qmark($b) > rebel_questmark 100059
        at $b echoat $b [1;35m°Q°f¾Ôª§¸¨¹õ                 §¹¦¨[0m
else
        at $b echoat $b [1;32m°Q°f¾Ôª§¸¨¹õ                 ¥¼§¹[0m
endif
if is_qmark($b) == rebel_questmark 100066
        at $b echoat $b [1;30m(¯µ)[0m[1;35mÁ{²×ªººëÆF»P¶Â·t«½«½     §¹¦¨[0m
endif
def_rembch null-tar
~
>room_defun q03~
at $b echoat $b .             [1;36m[¥v¥d©Ô¨©¦èÃä°Ï°ì I][0m
at $b echoat $b .             [1;37m{»È³DªZ¤h³ù}[0m
if class($b) == 11
or class($b) == 12
or class($b) == 13
        at $b echoat $b [1;30m(¯µ)[37m¾Ô¤h¶i¶¥¥ô°È:[0m
if is_qmark($b) & warrior_quest 100001
or is_qmark($b) & warrior_quest 100002
or is_qmark($b) & warrior_quest 100004
        at $b echoat $b [1;35m--¾Ô¤h¨t³Ì²×¸Õ·Ò             §¹¦¨[0m
else
        at $b echoat $b [1;32m--¾Ô¤h¨t³Ì²×¸Õ·Ò             ¥¼§¹[0m
endif
endif
if is_qmark($b) & warrior_quest 100008
        at $b echoat $b [1;35m§à¾Ü                         §¹¦¨[0m
else
        at $b echoat $b [1;32m§à¾Ü                         ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¦è¤è¶Â´ËªL}[0m
if is_qmark($b) == western_hermit 300005
        at $b echoat $b [1;35m´ËªLÁôªÌªº¦ÒÅç               §¹¦¨[0m
else
        at $b echoat $b [1;32m´ËªLÁôªÌªº¦ÒÅç               ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¥ì¦w¿Õ¿ò¸ñ}[0m
if is_qmark($b) >= ianua_supplies 100004
or is_qmark($b) & ianua_quest 100001
        at $b echoat $b [1;35mª«¸ê¹B°e I                   §¹¦¨[0m
else
        at $b echoat $b [1;32mª«¸ê¹B°e I                   ¥¼§¹[0m
endif
if is_qmark($b) >= ianua_supplies 100010
or is_qmark($b) & ianua_quest 100002
        at $b echoat $b [1;35m¸TÂ_¸ÕÅç                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¸TÂ_¸ÕÅç                     ¥¼§¹[0m
endif
if is_qmark($b) == ianua_teleporter
        at $b echoat $b [1;35m¶Ç°eªÌ¤§¥Û                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¶Ç°eªÌ¤§¥Û                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{ºñ¾ð¤ý°ê}[0m
if is_qmark($b) == greenwoods_war_over
        at $b echoat $b [1;35m¤d¨½¦@¼`®S                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¤d¨½¦@¼`®S                   ¥¼§¹[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100001
        at $b echoat $b [1;35mºñ¾ð­^¶¯²Ä¤@¬q               §¹¦¨[0m
else
        at $b echoat $b [1;32mºñ¾ð­^¶¯²Ä¤@¬q               ¥¼§¹[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100002
        at $b echoat $b [1;35mºñ¾ð­^¶¯²Ä¤G¬q               §¹¦¨[0m
else
        at $b echoat $b [1;32mºñ¾ð­^¶¯²Ä¤G¬q               ¥¼§¹[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100003
        at $b echoat $b [1;35mºñ¾ð­^¶¯²Ä¤T¬q               §¹¦¨[0m
else
        at $b echoat $b [1;32mºñ¾ð­^¶¯²Ä¤T¬q               ¥¼§¹[0m
endif
if is_qmark($b) == greenwoods_santa_brother
        at $b echoat $b [1;35m¸t¶ð³Â¥§¥d¥S§Ì·|             §¹¦¨[0m
else
        at $b echoat $b [1;32m¸t¶ð³Â¥§¥d¥S§Ì·|             ¥¼§¹[0m
endif
if is_qmark($b) == greenwoods_frowned_quest 100005
        at $b echoat $b [1;35m«CÀßªº·R±¡                   §¹¦¨[0m
else
        at $b echoat $b [1;32m«CÀßªº·R±¡                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{»jµïºô°Ï}[0m
if is_qmark($b) == arachnos_quest 100007
        at $b echoat $b [1;35mÀ°§U³Í­Û                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÀ°§U³Í­Û                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¤Ù¾Á°Ï}[0m
if is_qmark($b) & shire_quest 100002
        at $b echoat $b [1;35m¶ý¶ýªºª÷Âû³J(§äÂû)           §¹¦¨[0m
else
        at $b echoat $b [1;32m¶ý¶ýªºª÷Âû³J(§äÂû)           ¥¼§¹[0m
endif
if is_qmark($b) & shire_quest 100004
        at $b echoat $b [1;35m¶ý¶ýªºª÷Âû³J(¤U³J)           §¹¦¨[0m
else
        at $b echoat $b [1;32m¶ý¶ýªºª÷Âû³J(¤U³J)           ¥¼§¹[0m
endif
if is_qmark($b) & shire_quest 100001
        at $b echoat $b [1;35m¨gÀsÅ@²Å                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¨gÀsÅ@²Å                     ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q04~
at $b echoat $b .             [1;36m[¥v¥d©Ô¨©¦èÃä°Ï°ì II][0m
at $b echoat $b .             [1;37m{Å]ªk°ª¶ð}[0m
if is_qmark($b) & hitower_questmark 100001
        at $b echoat $b [1;35m¤ô´¹¬Á¼þªM                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¤ô´¹¬Á¼þªM                   ¥¼§¹[0m
endif
if is_qmark($b) & hitower_questmark 100002
        at $b echoat $b [1;35m¸T§Ò²Íºq                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¸T§Ò²Íºq                     ¥¼§¹[0m
endif
if race($b) == Drow
if is_qmark($b) & hitower_questmark 100004
        at $b echoat $b [1;35m¤»¨~¬Pªº¨ª¬õ¤§Ál             §¹¦¨[0m
else
        at $b echoat $b [1;32m¤»¨~¬Pªº¨ª¬õ¤§Ál             ¥¼§¹[0m
endif
endif
if is_qmark($b) & hitower_questmark 100008
        at $b echoat $b [1;35m»R­µ»Pµµ¹q                   §¹¦¨[0m
else
        at $b echoat $b [1;32m»R­µ»Pµµ¹q                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{«Õ­ß²`²W}[0m
if is_qmark($b) & murky_quest 100001
        at $b echoat $b [1;35m«ä¶mªº¹C¤l                   §¹¦¨[0m
else
        at $b echoat $b [1;32m«ä¶mªº¹C¤l                   ¥¼§¹[0m
endif
if is_qmark($b) & murky_quest 100002
        at $b echoat $b [1;35m¨ü¶Ëªº¤p¨k«Ä                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¨ü¶Ëªº¤p¨k«Ä                 ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¸TÂ_¤§¶ð}[0m
if is_qmark($b) == taboo_darkside 100001
        at $b echoat $b [1;35m(¯µ)¾Ô¤k¯«-¤a¸¦¨È¤§¼Ä        §¹¦¨[0m
else
if race($b) != Vampire
if is_qmark($b) == taboo_quest 100011
        at $b echoat $b [1;35mÁÙµ¹¤½¥D¯u¨­                 §¹¦¨[0m
else
        at $b echoat $b [1;32mÁÙµ¹¤½¥D¯u¨­                 ¥¼§¹[0m
endif
endif
endif
if is_qmark($b) == taboo_scroll 100002
        at $b echoat $b [1;35m±a§JÃ¹¦^®a                   §¹¦¨[0m
else
        at $b echoat $b [1;32m±a§JÃ¹¦^®a                   ¥¼§¹[0m
endif
if is_qmark($b) == taboo_fapei_death
        at $b echoat $b [1;35mª¢¬õ¶ð¶Ç»¡                   §¹¦¨[0m
else
        at $b echoat $b [1;32mª¢¬õ¶ð¶Ç»¡                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{®ü°ì}[0m
if is_qmark($b) == ocean_captain_quest 300003
or is_qmark($b) & mistocean_quest 100001
        at $b echoat $b [1;35m®ÉªÅ¸t¾Ô¤T³¡¦±(¤@)           §¹¦¨[0m
else
        at $b echoat $b [1;32m®ÉªÅ¸t¾Ô¤T³¡¦±(¤@)           ¥¼§¹[0m
endif
if is_qmark($b) == ocean_cleo_help 100007
or is_qmark($b) == ocean_cleo_help 100009
or is_qmark($b) & mistocean_quest 100002
or is_qmark($b) & mistocean_quest 100004
        at $b echoat $b [1;35m§J¨½¼Úªº©e°U                 §¹¦¨[0m
else
        at $b echoat $b [1;32m§J¨½¼Úªº©e°U                 ¥¼§¹[0m
endif
if is_qmark($b) == ocean_alc_stone 300003
or is_qmark($b) & mistocean_quest 100008
        at $b echoat $b [1;35m½åªÌ¤§¥Û                     §¹¦¨[0m
else
        at $b echoat $b [1;32m½åªÌ¤§¥Û                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{®ü¬v}[0m
if is_qmark($b) == ocean_potion_over 100001
or is_qmark($b) & ocean_quest 300001
        at $b echoat $b [1;35m»s³yÃÄ¤ô                     §¹¦¨[0m
else
        at $b echoat $b [1;32m»s³yÃÄ¤ô                     ¥¼§¹[0m
endif
if is_qmark($b) == ocean_quest 200008
or is_qmark($b) & ocean_quest 300002
        at $b echoat $b [1;35m®ø·À®ü©Ç                     §¹¦¨[0m
else
        at $b echoat $b [1;32m®ø·À®ü©Ç                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¤ÖªL¦x}[0m
if is_qmark($b) == shaolin_quest 200003
        at $b echoat $b [1;35m°kº»´c¹¬                     §¹¦¨[0m
else
        at $b echoat $b [1;32m°kº»´c¹¬                     ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q05~
at $b echoat $b .             [1;36m[¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï I][0m
at $b echoat $b .             [1;37m{¯«¸t¾ðªL}[0m
if is_qmark($b) & grove_quest 100001
        at $b echoat $b [1;35m¦ÑÀY¨Ó¦«¹Ú                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¦ÑÀY¨Ó¦«¹Ú                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{ºëÆF¤§´Ë}[0m
if is_qmark($b) == elftown_quest_over 100001
        at $b echoat $b [1;35mºëÆF¨M¾Ô¦aÆF                 §¹¦¨[0m
else
        at $b echoat $b [1;32mºëÆF¨M¾Ô¦aÆF                 ¥¼§¹[0m
endif
if is_qmark($b) == war_reward 300100
        at $b echoat $b [1;35mºëÆF­^¶¯                     §¹¦¨[0m
else
        at $b echoat $b [1;32mºëÆF­^¶¯                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¦aÆF¬}¸]}[0m
if is_qmark($b) & kobold_cave_hero 300512
        at $b echoat $b [1;35m¦aÆF­^¶¯                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¦aÆF­^¶¯                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{ÁÉ¥ï´µ©ì´¶}[0m
if is_qmark($b) & hobbit_all_quests 100016
        at $b echoat $b [1;35mµs¸é¤j®v                     §¹¦¨[0m
else
        at $b echoat $b [1;32mµs¸é¤j®v                     ¥¼§¹[0m
endif
if is_qmark($b) & hobbit_all_quests 100008
        at $b echoat $b [1;35m±¡²zªkªº§à¾Ü                 §¹¦¨[0m
else
        at $b echoat $b [1;32m±¡²zªkªº§à¾Ü                 ¥¼§¹[0m
endif
if is_qmark($b) & hobbit_all_quests 100001
        at $b echoat $b [1;35m¤j³°®È¦æ¦n¤â I               §¹¦¨[0m
else
        at $b echoat $b [1;32m¤j³°®È¦æ¦n¤â I               ¥¼§¹[0m
endif
if is_qmark($b) & hobbit_all_quests 100002
        at $b echoat $b [1;35m¤j³°®È¦æ¦n¤â II              §¹¦¨[0m
else
        at $b echoat $b [1;32m¤j³°®È¦æ¦n¤â II              ¥¼§¹[0m
endif
if is_qmark($b) & hobbit_all_quests 100004
        at $b echoat $b [1;35mÅ]§Ù                         §¹¦¨[0m
else
        at $b echoat $b [1;32mÅ]§Ù                         ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{®õ¬¥´µ}[0m
if is_qmark($b) == thalos_practice_quest 100001
        at $b echoat $b [1;35m¤jÅ]¾Éªº­Wµh                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤jÅ]¾Éªº­Wµh                 ¥¼§¹[0m
endif
if is_qmark($b) == thalos_mayor_quest 100005
        at $b echoat $b [1;35m¦u½Ã¶¤ªøªºÄb®¬               §¹¦¨[0m
else
        at $b echoat $b [1;32m¦u½Ã¶¤ªøªºÄb®¬               ¥¼§¹[0m
endif
if is_qmark($b) & thalos_darkforce_quest 132768
or is_qmark($b) & thalos_darkforce_quest 165536
        at $b echoat $b [1;35m®ø·À¶Â·t¶Õ¤O                 §¹¦¨[0m
else
        at $b echoat $b [1;32m®ø·À¶Â·t¶Õ¤O                 ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¤×®Ú¤ý°ê}[0m
if is_qmark($b) == juargan_quest 100012
        at $b echoat $b [1;35m¨ú¦^Åv§ú                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¨ú¦^Åv§ú                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¤H°¨§ø}[0m
if is_qmark($b) == wyvern_tower 100011
        at $b echoat $b [1;35m²M°£­¸Às¶ð                   §¹¦¨[0m
else
        at $b echoat $b [1;32m²M°£­¸Às¶ð                   ¥¼§¹[0m
endif
if is_qmark($b) == wyvern_tower 200016
        at $b echoat $b [1;35mÀs»h¥X¨Sª`·N                 §¹¦¨[0m
else
        at $b echoat $b [1;32mÀs»h¥X¨Sª`·N                 ¥¼§¹[0m
endif
if is_qmark($b) == wyvern_tower 300031
        at $b echoat $b [1;35mÆF»îÃìµ²                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÆF»îÃìµ²                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¸G¤H³®¹Ó}[0m
if is_qmark($b) == cata_quest2_explorer 100005
        at $b echoat $b [1;35m¬ã¨s­ûªº¬ã¨s                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¬ã¨s­ûªº¬ã¨s                 ¥¼§¹[0m
endif
if is_qmark($b) == cata_quest_templar 100006
        at $b echoat $b [1;35m¸t·µªº¨D±Ï                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¸t·µªº¨D±Ï                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¤H°¸Âí}[0m
if is_qmark($b) == muppet_grover_save 100006
        at $b echoat $b [1;35m¬@±Ï¤H°¸Âí                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¬@±Ï¤H°¸Âí                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{®É«B¦a¹Ó}[0m
if is_qmark($b) & cemetery_mark2 200002
        at $b echoat $b [1;35m¶Å°ÈªÈ¯É                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¶Å°ÈªÈ¯É                     ¥¼§¹[0m
endif
if is_qmark($b) & cemetery_mark2 200004
        at $b echoat $b [1;35m¦ºªÌ«è©À                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¦ºªÌ«è©À                     ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q06~
at $b echoat $b .             [1;36m[¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï II][0m
at $b echoat $b .             [1;37m{¥Ã«í¤§¶ð}[0m
if class($b) == 0
or class($b) == 1
or class($b) == 5
or class($b) == 6
or class($b) == 7
or class($b) == 8
or class($b) == 15
or class($b) == 16
or class($b) == 14
        at $b echoat $b [1;30m(¯µ)[37mªk®v°ò¥»¥ô°È:[0m
if is_qmark($b) & eternity_quest 100001
        at $b echoat $b [1;35m--Å]ªkªì¯ÅÀË©w               §¹¦¨[0m
else
        at $b echoat $b [1;32m--Å]ªkªì¯ÅÀË©w               ¥¼§¹[0m
endif
endif
if is_qmark($b) & eternity_quest 100002
        at $b echoat $b [1;35m¤÷¿Ëªºª÷¿ö                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¤÷¿Ëªºª÷¿ö                   ¥¼§¹[0m
endif
if is_qmark($b) & eternity_quest 100004
        at $b echoat $b [1;35m§xÃøªº§@·~                   §¹¦¨[0m
else
        at $b echoat $b [1;32m§xÃøªº§@·~                   ¥¼§¹[0m
endif
if is_qmark($b) & eternity_quest 100008
        at $b echoat $b [1;35m¹C¤l§u                       §¹¦¨[0m
else
        at $b echoat $b [1;32m¹C¤l§u                       ¥¼§¹[0m
endif
if is_qmark($b) & eternity_quest 100016
        at $b echoat $b [1;35m¨g¤H­Ìªºª§°õ                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¨g¤H­Ìªºª§°õ                 ¥¼§¹[0m
endif
if is_qmark($b) & eternity_quest 100032
        at $b echoat $b [1;35m¤­¨~¥l³ê°}                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¤­¨~¥l³ê°}                   ¥¼§¹[0m
endif
if class($b) == 0
or class($b) == 5
or class($b) == 6
or class($b) == 15
        at $b echoat $b [1;30m(¯µ)[37m²ÕÂ´¥ô°È:[0m
if is_qmark($b) >= wfs_clan_member 100001
        at $b echoat $b [1;35m--­·¤õ®ü²{¥@                 §¹¦¨[0m
else
        at $b echoat $b [1;32m--­·¤õ®ü²{¥@                 ¥¼§¹[0m
endif
endif
if is_qmark($b) & eternity_quest 100064
        at $b echoat $b [1;35m»s³yÅ]¤Oªk§ú                 §¹¦¨[0m
else
        at $b echoat $b [1;32m»s³yÅ]¤Oªk§ú                 ¥¼§¹[0m
endif
if is_qmark($b) & eternity_quest 100128
        at $b echoat $b [1;35m¦M¾÷                         §¹¦¨[0m
else
        at $b echoat $b [1;32m¦M¾÷                         ¥¼§¹[0m
endif
if is_qmark($b) & eternity_quest 100256
        at $b echoat $b [1;35m¤pºëÆFªºÄ@±æ                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤pºëÆFªºÄ@±æ                 ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{°Ò¦«º¸³ùÂS}[0m
if is_qmark($b) != mahntor_ogre_quest2 100017
if is_qmark($b) == mahntor_ogre_quest1 200004
or is_qmark($b) == mahntor_ogre_quest1 100004
        at $b echoat $b [1;35m­©ªøªº¤ß¨Æ                   §¹¦¨[0m
else
        at $b echoat $b [1;32m­©ªøªº¤ß¨Æ                   ¥¼§¹[0m
endif
endif
if is_qmark($b) != mahntor_ogre_quest2 100017
if is_qmark($b) == mahntor_shaman_nut 100004
        at $b echoat $b [1;35m¦Ñ¦~·ö§bªº§ÅÂå               §¹¦¨[0m
else
        at $b echoat $b [1;32m¦Ñ¦~·ö§bªº§ÅÂå               ¥¼§¹[0m
endif
endif
if is_qmark($b) != mahntor_ogre_quest1 200004
if is_qmark($b) != mahntor_ogre_quest1 100004
if is_qmark($b) == mahntor_ogre_quest2 100017
        at $b echoat $b [1;35m³ùÂSªº¦M¾÷                   §¹¦¨[0m
else
        at $b echoat $b [1;32m³ùÂSªº¦M¾÷                   ¥¼§¹[0m
endif
endif
endif
at $b echoat $b .             [1;37m{ÆZ¯î´ËªL}[0m
if is_qmark($b) == forest_quest
        at $b echoat $b [1;35m¾Ç²ß¤g¤H»y¨¥                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¾Ç²ß¤g¤H»y¨¥                 ¥¼§¹[0m
endif
if is_qmark($b) == mist_clean_quest 100020
        at $b echoat $b [1;35m¥Í¯fªº¤g¤H                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¥Í¯fªº¤g¤H                   ¥¼§¹[0m
endif
if is_qmark($b) & forest_quest 300128
        at $b echoat $b [1;35m­©ªøªº°U¥I                   §¹¦¨[0m
else
        at $b echoat $b [1;32m­©ªøªº°U¥I                   ¥¼§¹[0m
endif
if is_qmark($b) >= forest_quest 100004
        at $b echoat $b [1;35m¤õµK¯«¯]                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¤õµK¯«¯]                     ¥¼§¹[0m
endif
if is_qmark($b) == forest_quest 100007
or is_qmark($b) & forest_quest 101024
        at $b echoat $b [1;35m¤õÀsÁÉ¤Ú¬¥¥q                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤õÀsÁÉ¤Ú¬¥¥q                 ¥¼§¹[0m
endif
if class($b) == 14
or class($b) == 18
or class($b) == 19
        at $b echoat $b [1;30m(¯µ)[37mÆZ«L¸Õ·Ò¥ô°È:[0m
if is_qmark($b) == forest_barbarian_exam 100002
        at $b echoat $b [1;35m--³¥ÆZ¤H¸Õ·Ò                 §¹¦¨[0m
else
        at $b echoat $b [1;32m--³¥ÆZ¤H¸Õ·Ò                 ¥¼§¹[0m
endif
endif
at $b echoat $b .             [1;37m{³Á®æ«°}[0m
if is_qmark($b) == mega1_arena_owner 300001
        at $b echoat $b [1;35mÄv§Þ³õ¤§Á¼                   §¹¦¨[0m
else
        at $b echoat $b [1;32mÄv§Þ³õ¤§Á¼                   ¥¼§¹[0m
endif
if is_qmark($b) == mega1_alibaba_hunting 100001
        at $b echoat $b [1;35mÂy±þªü¨½¤Ú¤Ú                 §¹¦¨[0m
else
        at $b echoat $b [1;32mÂy±þªü¨½¤Ú¤Ú                 ¥¼§¹[0m
endif
if is_qmark($b) == mega1_mayor_vsjudge 100004
        at $b echoat $b [1;35m¨©¦hªº¤â§ú                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¨©¦hªº¤â§ú                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¦ã´µ¨©º¸¶Ä§L¹ÎÀç¦a}[0m
if is_qmark($b) & sandrock_questmark 100001
        at $b echoat $b [1;35m²§¬É»y¨¥                     §¹¦¨[0m
else
        at $b echoat $b [1;32m²§¬É»y¨¥                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100002
        at $b echoat $b [1;35mª÷ÄÝ¦¬ÁÊ                     §¹¦¨[0m
else
        at $b echoat $b [1;32mª÷ÄÝ¦¬ÁÊ                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100004
        at $b echoat $b [1;35m§L¾¹Å±³y                     §¹¦¨[0m
else
        at $b echoat $b [1;32m§L¾¹Å±³y                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100008
        at $b echoat $b [1;35mÅ]ªkºP¯È                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÅ]ªkºP¯È                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100016
        at $b echoat $b [1;35m¹}®Æ½Õ°t                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¹}®Æ½Õ°t                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100032
        at $b echoat $b [1;35m¬\¤«½Õ¬d                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¬\¤«½Õ¬d                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100064
        at $b echoat $b [1;35m¦w¼¾½Ð¨D                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¦w¼¾½Ð¨D                     ¥¼§¹[0m
endif
if is_qmark($b) & sandrock_questmark 100128
        at $b echoat $b [1;35m´M§ä¤u¨ã                     §¹¦¨[0m
else
        at $b echoat $b [1;32m´M§ä¤u¨ã                     ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q07~
at $b echoat $b .             [1;36m[¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï I][0m
at $b echoat $b .             [1;37m{³Ð¥@¬ö}[0m
if is_qmark($b) >= ultima_destroy_evil 100008
        at $b echoat $b [1;35m¤K¤j¬ü¼wªº¦ÒÅç               §¹¦¨[0m
else
        at $b echoat $b [1;32m¤K¤j¬ü¼wªº¦ÒÅç               ¥¼§¹[0m
endif
if is_qmark($b) == ultima_destroy_evil 100011
        at $b echoat $b [1;35m³Ð¥@¬ö¸t¾Ô                   §¹¦¨[0m
else
        at $b echoat $b [1;32m³Ð¥@¬ö¸t¾Ô                   ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¶øªL©¬´µ¤s}[0m
if is_qmark($b) == olympus_weapon_quest 200009
or is_qmark($b) == olympus_weapon_quest 100009
        at $b echoat $b [1;35m¤Ñ¬É¤§ª§                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¤Ñ¬É¤§ª§                     ¥¼§¹[0m
endif
if is_qmark($b) == olympus_jail_quest 100021
        at $b echoat $b [1;35mÄÀ©ñ¥}¥Ç                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÄÀ©ñ¥}¥Ç                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¸t°ì}[0m
if is_qmark($b) == saintarea_quest_main 100040
        at $b echoat $b [1;35m¶Àª÷¤Q¤G®c¦ÒÅç               §¹¦¨[0m
else
        at $b echoat $b [1;32m¶Àª÷¤Q¤G®c¦ÒÅç               ¥¼§¹[0m
endif
if is_qmark($b) == saintarea_quest_geminimask 100002
        at $b echoat $b [1;35m¥´³y¸t¦ç                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¥´³y¸t¦ç                     ¥¼§¹[0m
endif
if is_qmark($b) == saintarea_perseus 100019
        at $b echoat $b [1;35m¥´³y¯«¬Þ                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¥´³y¯«¬Þ                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¬P¬É}[0m
if is_qmark($b) & astral_quest 100001
        at $b echoat $b [1;35m¬@±Ï¬P¬É                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¬@±Ï¬P¬É                     ¥¼§¹[0m
endif
if is_qmark($b) & astral_quest 100002
        at $b echoat $b [1;35m¬@±Ï¬õÀs                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¬@±Ï¬õÀs                     ¥¼§¹[0m
endif
if is_qmark($b) & astral_quest 100004
        at $b echoat $b [1;35m¥´³yÅ@¥Ò                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¥´³yÅ@¥Ò                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¬P¥ú¤§«°}[0m
if is_qmark($b) & thoran_quest 100001
        at $b echoat $b [1;35m´©±Ï©`¨È                     §¹¦¨[0m
else
        at $b echoat $b [1;32m´©±Ï©`¨È                     ¥¼§¹[0m
endif
if is_qmark($b) & thoran_quest 100002
        at $b echoat $b [1;35m¥D³ù³q¦æÃÒ                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¥D³ù³q¦æÃÒ                   ¥¼§¹[0m
endif
if is_qmark($b) & thoran_quest 100004
        at $b echoat $b [1;35m°s©±ªZ¤hªº¸q²´               §¹¦¨[0m
else
        at $b echoat $b [1;32m°s©±ªZ¤hªº¸q²´               ¥¼§¹[0m
endif
if is_qmark($b) & thoran_quest 100008
        at $b echoat $b [1;35m¼~¶Ë¦u½ÃªºÄ@±æ               §¹¦¨[0m
else
        at $b echoat $b [1;32m¼~¶Ë¦u½ÃªºÄ@±æ               ¥¼§¹[0m
endif
if is_qmark($b) & thoran_quest 100016
        at $b echoat $b [1;35m³Q¤U¶A©Gªº³ù¥D               §¹¦¨[0m
else
        at $b echoat $b [1;32m³Q¤U¶A©Gªº³ù¥D               ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q08~
at $b echoat $b .             [1;36m[¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï II][0m
at $b echoat $b .             [1;37m{«n·¥¤j³°}[0m
if is_qmark($b) & southend_quest 100002
        at $b echoat $b [1;35m¥¬¥¬ªº³g¦Y                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¥¬¥¬ªº³g¦Y                   ¥¼§¹[0m
endif
if is_qmark($b) & southend_quest 100004
        at $b echoat $b [1;35mªÖ¯Sªº·Ð´o                   §¹¦¨[0m
else
        at $b echoat $b [1;32mªÖ¯Sªº·Ð´o                   ¥¼§¹[0m
endif
if is_qmark($b) & southend_quest 100008
        at $b echoat $b [1;35m§ø²øªº¦M¾÷                   §¹¦¨[0m
else
        at $b echoat $b [1;32m§ø²øªº¦M¾÷                   ¥¼§¹[0m
endif
if is_qmark($b) & southend_quest 100016
        at $b echoat $b [1;35mº¿²úªº°g½k                   §¹¦¨[0m
else
        at $b echoat $b [1;32mº¿²úªº°g½k                   ¥¼§¹[0m
endif
if is_qmark($b) & southend_quest 100032
        at $b echoat $b [1;35m¯«¾¹ªº­«²{                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¯«¾¹ªº­«²{                   ¥¼§¹[0m
endif
if is_qmark($b) & southend_quest 100064
        at $b echoat $b [1;35m´o¤H¤p¯T¤ü                   §¹¦¨[0m
else
        at $b echoat $b [1;32m´o¤H¤p¯T¤ü                   ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q09~
at $b echoat $b .             [1;36m[°gÃú¤§®q¥D°Ï][0m
at $b echoat $b .             [1;37m{¥ú©ú¤§Âí}[0m
if is_qmark($b) & mist_quest 100001
        at $b echoat $b [1;35m»s³yÃÄ¤ô                     §¹¦¨[0m
else
        at $b echoat $b [1;32m»s³yÃÄ¤ô                     ¥¼§¹[0m
endif
if is_qmark($b) & mist_quest 100002
        at $b echoat $b [1;35m¦º¤`¨¦¤§®È                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¦º¤`¨¦¤§®È                   ¥¼§¹[0m
endif
if is_qmark($b) & mist_quest 100004
        at $b echoat $b [1;35m¥ú©ú¤§Âí©U§£¨Æ¥ó             §¹¦¨[0m
else
        at $b echoat $b [1;32m¥ú©ú¤§Âí©U§£¨Æ¥ó             ¥¼§¹[0m
endif
if is_qmark($b) == mist_protector 100001
        at $b echoat $b [1;35m®ø·À®£Às                     §¹¦¨[0m
else
        at $b echoat $b [1;32m®ø·À®£Às                     ¥¼§¹[0m
endif
if is_qmark($b) & mist_council_over 100002
        at $b echoat $b [1;35m¥«Ä³·|¥ô°È--Â²©ö°e³f I       §¹¦¨[0m
else
        at $b echoat $b [1;32m¥«Ä³·|¥ô°È--Â²©ö°e³f I       ¥¼§¹[0m
endif
if is_qmark($b) & mist_council_over 100004
        at $b echoat $b [1;35m¥«Ä³·|¥ô°È--Â²©ö°e³f II      §¹¦¨[0m
else
        at $b echoat $b [1;32m¥«Ä³·|¥ô°È--Â²©ö°e³f II      ¥¼§¹[0m
endif
if is_qmark($b) & mist_council_over 100008
        at $b echoat $b [1;35m¥«Ä³·|¥ô°È--·t±þ I           §¹¦¨[0m
else
        at $b echoat $b [1;32m¥«Ä³·|¥ô°È--·t±þ I           ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¤ô´¹´ò}[0m
if is_qmark($b) >= crystal_reward 100001
        at $b echoat $b [1;35m¬@±Ï»ÈÀs                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¬@±Ï»ÈÀs                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{·¬¬õªL}[0m
if is_qmark($b) == maple_ack 100004
        at $b echoat $b [1;35m¦ÛµMªº©w«ß                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¦ÛµMªº©w«ß                   ¥¼§¹[0m
endif
if is_qmark($b) == maple_camus_attack 100003
        at $b echoat $b [1;35m«Õ­ß²`²Wªº¨Ï®{               §¹¦¨[0m
else
        at $b echoat $b [1;32m«Õ­ß²`²Wªº¨Ï®{               ¥¼§¹[0m
endif
if class($b) == 12
        at $b echoat $b [1;30m(¯µ)[37m¼C¤h¶i¶¥¥ô°È:[0m
if is_qmark($b) == maple_swd_trail 100006
        at $b echoat $b [1;35m--¼C¤h¶i¶¥¸Õ·Ò               §¹¦¨[0m
else
        at $b echoat $b [1;32m--¼C¤h¶i¶¥¸Õ·Ò               ¥¼§¹[0m
endif
endif
if class($b) == 2
or class($b) == 9
or class($b) == 10
or class($b) == 17
at $b echoat $b .             [1;37m{µs¸é­n¶ë}[0m
if is_qmark($b) >= bandit_thief_exam 100005
        at $b echoat $b [1;35mµs¸é¦u«h                     §¹¦¨[0m
else
        at $b echoat $b [1;32mµs¸é¦u«h                     ¥¼§¹[0m
endif
endif
if class($b) == 10
        at $b echoat $b [1;30m(¯µ)[37m§ÔªÌ¶i¶¥¥ô°È:[0m
if is_qmark($b) == bandit_thief_exam 100007
        at $b echoat $b [1;35m--§ÔªÌ¸Õ·Ò                   §¹¦¨[0m
else
        at $b echoat $b [1;32m--§ÔªÌ¸Õ·Ò                   ¥¼§¹[0m
endif
endif
def_rembch null-tar
~
>room_defun q10~
at $b echoat $b .             [1;36m[°gÃú¤§®q©`¹Ï¤ý°ê][0m
at $b echoat $b .             [1;37m{©`¹Ï«°³ù}[0m
if is_qmark($b) >= naytil_quest 100008
        at $b echoat $b [1;35m¹Ï®ÑÀ]¤¤ªº´cÅ]               §¹¦¨[0m
else
        at $b echoat $b [1;32m¹Ï®ÑÀ]¤¤ªº´cÅ]               ¥¼§¹[0m
endif
if is_qmark($b) != fight_naytil 100008
if is_qmark($b) == fight_highland 100005
        at $b echoat $b [1;35m¨M¾Ô°ª¦a                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¨M¾Ô°ª¦a                     ¥¼§¹[0m
endif
endif
if is_qmark($b) == naytil_princess 100001
        at $b echoat $b [1;35m´M§ä¤p¿ß                     §¹¦¨[0m
else
        at $b echoat $b [1;32m´M§ä¤p¿ß                     ¥¼§¹[0m
endif
if is_qmark($b) == naytil_rebels 200001
or is_qmark($b) == naytil_rebels 200002
        at $b echoat $b [1;35m©`¹Ï/¤Ï«q­x¤j¾Ô½g            §¹¦¨[0m
else
        at $b echoat $b [1;32m©`¹Ï/¤Ï«q­x¤j¾Ô½g            ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{°ª¦a}[0m
if is_qmark($b) != fight_highland 100005
if is_qmark($b) == fight_naytil 100008
        at $b echoat $b [1;35m¨M¾Ô©`¹Ï                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¨M¾Ô©`¹Ï                     ¥¼§¹[0m
endif
endif
if is_qmark($b) == for_rose 100005
        at $b echoat $b [1;35mª´ºÀ¤§®Ñ                     §¹¦¨[0m
else
        at $b echoat $b [1;32mª´ºÀ¤§®Ñ                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{¯Á¹p´µ«°}[0m
if is_qmark($b) & mist_quest 100008
        at $b echoat $b [1;35m«_ÀIªÌªºÄ@±æ                 §¹¦¨[0m
else
        at $b echoat $b [1;32m«_ÀIªÌªºÄ@±æ                 ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{°­Âí}[0m
if is_qmark($b) & gtown_quest_list 100001
        at $b echoat $b [1;35m¦º¤`¨¦¤J«I                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¦º¤`¨¦¤J«I                   ¥¼§¹[0m
endif
if race($b) == vampire
if is_qmark($b) == gtown_vampire_finish
        at $b echoat $b [1;35m§l¦å°­¤§ÅÊ                   §¹¦¨[0m
else
        at $b echoat $b [1;32m§l¦å°­¤§ÅÊ                   ¥¼§¹[0m
endif
endif
if is_qmark($b) & gtown_quest_list 100002
        at $b echoat $b [1;35m¤p³c¤T¥S§Ì¤§¤Í               §¹¦¨[0m
else
        at $b echoat $b [1;32m¤p³c¤T¥S§Ì¤§¤Í               ¥¼§¹[0m
endif
if is_qmark($b) & gtown_quest_list 100004
        at $b echoat $b [1;35m¹Ú¤Û¤§ªá                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¹Ú¤Û¤§ªá                     ¥¼§¹[0m
endif
if is_qmark($b) & gtown_quest_list 100008
        at $b echoat $b [1;35m¤p»¡®aªº½Ï¥Í                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤p»¡®aªº½Ï¥Í                 ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q11~
at $b echoat $b .             [1;36m[°gÃú¤§®q¤§¨ä¥L°Ï°ì I][0m
at $b echoat $b .             [1;37m{»·¥j´ËªL}[0m
if is_qmark($b) & prehist_quest 100001
        at $b echoat $b [1;35m¬@±Ï³¥Ã~                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¬@±Ï³¥Ã~                     ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100002
        at $b echoat $b [1;35m¨ú®®¦Bªø¦Ñ                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¨ú®®¦Bªø¦Ñ                   ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100004
        at $b echoat $b [1;35m¬°¤pµl¨ú±o°t¤M               §¹¦¨[0m
else
        at $b echoat $b [1;32m¬°¤pµl¨ú±o°t¤M               ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100008
        at $b echoat $b [1;35mÀ°¸ô§J­×²z©Ð¤l               §¹¦¨[0m
else
        at $b echoat $b [1;32mÀ°¸ô§J­×²z©Ð¤l               ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100016
        at $b echoat $b [1;35m¤p¤k«ÄªºÄ_ÂÃ                 §¹¦¨[0m
else
        at $b echoat $b [1;32m¤p¤k«ÄªºÄ_ÂÃ                 ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100032
        at $b echoat $b [1;35mÀ°§U»·¥j´ËªL¤¤«_ÀIªÌ²æÂ÷     §¹¦¨[0m
else
        at $b echoat $b [1;32mÀ°§U»·¥j´ËªL¤¤«_ÀIªÌ²æÂ÷     ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100064
        at $b echoat $b [1;35m¨ú±o¿W²´³½»P¤jÀJ³J           §¹¦¨[0m
else
        at $b echoat $b [1;32m¨ú±o¿W²´³½»P¤jÀJ³J           ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100128
        at $b echoat $b [1;35m²M°£¦B®w¤º¥Íª«               §¹¦¨[0m
else
        at $b echoat $b [1;32m²M°£¦B®w¤º¥Íª«               ¥¼§¹[0m
endif
if is_qmark($b) & prehist_quest 100512
        at $b echoat $b [1;35mÃú¦Ñ®vªº·Ð´o                 §¹¦¨[0m
else
        at $b echoat $b [1;32mÃú¦Ñ®vªº·Ð´o                 ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{«ÊÅ]¬}¸]}[0m
if class($b) == 7
or class($b) == 8
if is_qmark($b) & sealevil_quest 100001
        at $b echoat $b [1;35m¦uÅ@ªÌ´¹¥Û                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¦uÅ@ªÌ´¹¥Û                   ¥¼§¹[0m
endif
endif
if is_qmark($b) & sealevil_quest 100002
or is_qmark($b) > sealevil_Aleru_quest 100018
        at $b echoat $b [1;35m®a±Úªº¨Ï©R²Ä¤@¬q             §¹¦¨[0m
else
        at $b echoat $b [1;32m®a±Úªº¨Ï©R²Ä¤@¬q             ¥¼§¹[0m
endif
if is_qmark($b) & sealevil_quest 100002
        at $b echoat $b [1;35m®a±Úªº¨Ï©R²Ä¤G¬q             §¹¦¨[0m
else
        at $b echoat $b [1;32m®a±Úªº¨Ï©R²Ä¤G¬q             ¥¼§¹[0m
endif
if is_qmark($b) & sealevil_quest 100004
        at $b echoat $b [1;35m©n©f®¦¤³¿ý                   §¹¦¨[0m
else
        at $b echoat $b [1;32m©n©f®¦¤³¿ý                   ¥¼§¹[0m
endif
if is_qmark($b) & sealevil_quest 100008
        at $b echoat $b [1;35mºëÆF¤§Æp                     §¹¦¨[0m
else
        at $b echoat $b [1;32mºëÆF¤§Æp                     ¥¼§¹[0m
endif
if is_qmark($b) & sealevil_quest 100016
        at $b echoat $b [1;35mÂù¤b¦X¤@                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÂù¤b¦X¤@                     ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{³·µØ«°³ù}[0m
if is_qmark($b) & snow_questcheck 100001
or is_qmark($b) == snow_Porak01_quest 100011
        at $b echoat $b [1;35m¯µ±K¶¡¿Ò                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¯µ±K¶¡¿Ò                     ¥¼§¹[0m
endif
if is_qmark($b) & snow_questcheck 100002
or is_qmark($b) == snow_Porak02_quest 100010
        at $b echoat $b [1;35m±þ¤â                         §¹¦¨[0m
else
        at $b echoat $b [1;32m±þ¤â                         ¥¼§¹[0m
endif
if is_qmark($b) & snow_questcheck 100004
or is_qmark($b) == snow_Porak03_quest 100015
        at $b echoat $b [1;35m²Ä¥|¯«¾¹                     §¹¦¨[0m
else
        at $b echoat $b [1;32m²Ä¥|¯«¾¹                     ¥¼§¹[0m
endif
if is_qmark($b) & snow_questcheck 100008
or is_qmark($b) == sealevil_Tylas_quest 100015
        at $b echoat $b [1;35mºëÆF¤§ÆpII                   §¹¦¨[0m
else
        at $b echoat $b [1;32mºëÆF¤§ÆpII                   ¥¼§¹[0m
endif
if is_qmark($b) & snow_questcheck 100016
or is_qmark($b) >= sealevil_Aleru_quest 100089
        at $b echoat $b [1;35m®a±Úªº¨Ï©R²Ä¤T¬q             §¹¦¨[0m
else
        at $b echoat $b [1;32m®a±Úªº¨Ï©R²Ä¤T¬q             ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q12~
at $b echoat $b .             [1;36m[°gÃú¤§®q¤§¨ä¥L°Ï°ì II][0m
at $b echoat $b .             [1;37m{·R¼ä±öµá®q}[0m
if is_qmark($b) & Ageemephy_questmark 200001
        at $b echoat $b [1;35m¥¼­°Á{ªº¯«¿Ù¡EÁÀ¨¥           §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100001
                at $b echoat $b [1;35m¥¼­°Á{ªº¯«¿Ù                 §¹¦¨[0m
        else
                at $b echoat $b [1;32m¥¼­°Á{ªº¯«¿Ù                 ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200002
        at $b echoat $b [1;35m¥¢Â¾§L¥Ò©x¡E°O¾Ð             §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100002
                at $b echoat $b [1;35m¥¢Â¾§L¥Ò©x                   §¹¦¨[0m
        else
                at $b echoat $b [1;32m¥¢Â¾§L¥Ò©x                   ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200004
        at $b echoat $b [1;35m´cÅ]°«¤h¡E­I«q               §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100004
                at $b echoat $b [1;35m´cÅ]°«¤h                     §¹¦¨[0m
        else
                at $b echoat $b [1;32m´cÅ]°«¤h                     ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200008
        at $b echoat $b [1;35m¶ÂªZªÌ¤§¼C¡E±ÏÅ«             §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100008
                at $b echoat $b [1;35m¶ÂªZªÌ¤§¼C                   §¹¦¨[0m
        else
                at $b echoat $b [1;32m¶ÂªZªÌ¤§¼C                   ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200016
        at $b echoat $b [1;35m¸t¨¸¬Ò¨ãªº¤Ö¤k¡E¯e¨«         §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100016
                at $b echoat $b [1;35m¸t¨¸¬Ò¨ãªº¤Ö¤k               §¹¦¨[0m
        else
                at $b echoat $b [1;32m¸t¨¸¬Ò¨ãªº¤Ö¤k               ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200032
        at $b echoat $b [1;35mÅ]ªk°}¬ã¨s¡E´dÄ@             §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100032
                at $b echoat $b [1;35mÅ]ªk°}¬ã¨s                   §¹¦¨[0m
        else
                at $b echoat $b [1;32mÅ]ªk°}¬ã¨s                   ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200064
        at $b echoat $b [1;35m¥ý½å¦C¶Ç¡EºaÄ£               §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100064
                at $b echoat $b [1;35m¥ý½å¦C¶Ç                     §¹¦¨[0m
        else
                at $b echoat $b [1;32m¥ý½å¦C¶Ç                     ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200128
        at $b echoat $b [1;35m¨IºÎ¦bÀR¤îªº®É¥ú¤¤¡E¹Ú­µ     §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100128
                at $b echoat $b [1;35m¨IºÎ¦bÀR¤îªº®É¥ú¤¤           §¹¦¨[0m
        else
                at $b echoat $b [1;32m¨IºÎ¦bÀR¤îªº®É¥ú¤¤           ¥¼§¹[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200256
        at $b echoat $b [1;35m°l´M«e¤Hªº¸}¨B¡E­y¸ñ         §¹¦¨[0m
else
        if is_qmark($b) & Ageemephy_questmark 100256
                at $b echoat $b [1;35m°l´M«e¤Hªº¸}¨B               §¹¦¨[0m
        else
                at $b echoat $b [1;32m°l´M«e¤Hªº¸}¨B               ¥¼§¹[0m
        endif
endif
at $b echoat $b .             [1;37m{·R¼ä±öµá®q©PÃä}[0m
if is_qmark($b) & TideIsland_questmark 200001
        at $b echoat $b [1;35m³Q«Ê¦L¤§¥j²½¾Â               §¹¦¨[0m
else
        at $b echoat $b [1;32m³Q«Ê¦L¤§¥j²½¾Â               ¥¼§¹[0m
endif
if class($b) == 11
or class($b) == 16
if is_qmark($b) & TideIsland_questmark 200002
        at $b echoat $b [1;35mÂyºµ¶Ç»¡                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÂyºµ¶Ç»¡                     ¥¼§¹[0m
endif
endif
if is_qmark($b) >= sealevil_Aleru_quest 100104
        at $b echoat $b [1;35m®a±Úªº¨Ï©R²Ä¥|¬q             §¹¦¨[0m
else
        at $b echoat $b [1;32m®a±Úªº¨Ï©R²Ä¥|¬q             ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{«Õ·t±KªL}[0m
if is_qmark($b) == mirkwood_quest 100006
        at $b echoat $b [1;35m°kº»                         §¹¦¨[0m
else
        at $b echoat $b [1;32m°kº»                         ¥¼§¹[0m
endif
at $b echoat $b .             [1;37m{º¿©Ô°Ç«°}[0m
if is_qmark($b) >= malathar_quest 100003
        at $b echoat $b [1;35m¤p´c°­°»¹î³¡¶¤               §¹¦¨[0m
else
        at $b echoat $b [1;32m¤p´c°­°»¹î³¡¶¤               ¥¼§¹[0m
endif
if is_qmark($b) >= malathar_quest 100008
        at $b echoat $b [1;35mÅ]¤O®½Äm                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÅ]¤O®½Äm                     ¥¼§¹[0m
endif
if is_qmark($b) >= malathar_quest 100012
        at $b echoat $b [1;35m¶¡¿Ò²M°£¾Ô                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¶¡¿Ò²M°£¾Ô                   ¥¼§¹[0m
endif
if is_qmark($b) >= malathar_quest 100029
        at $b echoat $b [1;35mÄ³©M                         §¹¦¨[0m
else
        at $b echoat $b [1;32mÄ³©M                         ¥¼§¹[0m
endif
if is_qmark($b) >= malathar_quest 100033
        at $b echoat $b [1;35m²l¤õ®É¨è                     §¹¦¨[0m
else
        at $b echoat $b [1;32m²l¤õ®É¨è                     ¥¼§¹[0m
endif
if is_qmark($b) >= malathar_quest 100038
        at $b echoat $b [1;35m¯«Å@¤§¤O                     §¹¦¨[0m
else
        at $b echoat $b [1;32m¯«Å@¤§¤O                     ¥¼§¹[0m
endif
if is_qmark($b) == malathar_quest 100043
        at $b echoat $b [1;35m¶Â·tªº¹êÅé                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¶Â·tªº¹êÅé                   ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun q13~
at $b echoat $b .             [1;36m[°gÃú¤§®q°g´µ¨ô¿Õ°Ï°ì][0m
at $b echoat $b .             [1;37m{°g´µ¨ô¿Õ¨¦¦a}[0m
if is_qmark($b) >= myth_seamstress 100020
or is_qmark($b) & myth_greenleaf_end 100001
        at $b echoat $b [1;35mÂ¾¤Hºë¯«                     §¹¦¨[0m
else
        at $b echoat $b [1;32mÂ¾¤Hºë¯«                     ¥¼§¹[0m
endif
if is_qmark($b) & myth_greenleaf_end 100002
        at $b echoat $b [1;35mÅ]¤O¬É­­¬ð¯}¬ã¨s             §¹¦¨[0m
else
        at $b echoat $b [1;32mÅ]¤O¬É­­¬ð¯}¬ã¨s             ¥¼§¹[0m
endif
if is_qmark($b) & myth_greenleaf_end 100004
or is_qmark($b) & myth_greenleaf_end 200004
        at $b echoat $b [1;35mÂ¼¯Zªº¦æ¬Fªø©x               §¹¦¨[0m
else
        at $b echoat $b [1;32mÂ¼¯Zªº¦æ¬Fªø©x               ¥¼§¹[0m
endif
if is_qmark($b) & myth_family_recipe 300001
        at $b echoat $b [1;35m°l±¥¥ý¤H                     §¹¦¨[0m
else
        at $b echoat $b [1;32m°l±¥¥ý¤H                     ¥¼§¹[0m
endif
if is_qmark($b) & myth_family_recipe 300002
        at $b echoat $b [1;35mÀ³¨üÄ~©Ó¤§ª«                 §¹¦¨[0m
else
        at $b echoat $b [1;32mÀ³¨üÄ~©Ó¤§ª«                 ¥¼§¹[0m
endif
if is_qmark($b) & myth_elven_end 100001
or is_qmark($b) & myth_elven_end 200001
        at $b echoat $b [1;35m¥¢¸¨ªº½g³¹                   §¹¦¨[0m
else
        at $b echoat $b [1;32m¥¢¸¨ªº½g³¹                   ¥¼§¹[0m
endif
if is_qmark($b) & myth_templar_end 100001
or is_qmark($b) & myth_templar_end 200001
        at $b echoat $b [1;35m±Ð¹Î½Õ¬d¶¤                   §¹¦¨[0m
else
        at $b echoat $b [1;32m±Ð¹Î½Õ¬d¶¤                   ¥¼§¹[0m
endif
if is_qmark($b) & myth_templar_end 100002
or is_qmark($b) & myth_templar_end 200002
        at $b echoat $b [1;35m¦æÂÜ¤£©úªºÃM¤h               §¹¦¨[0m
else
        at $b echoat $b [1;32m¦æÂÜ¤£©úªºÃM¤h               ¥¼§¹[0m
endif
if is_qmark($b) & myth_templar_end 100004
or is_qmark($b) & myth_templar_end 200004
        at $b echoat $b [1;35m±Ð¹Î½Õ¬d¶¤II                 §¹¦¨[0m
else
        at $b echoat $b [1;32m±Ð¹Î½Õ¬d¶¤II                 ¥¼§¹[0m
endif
if is_qmark($b) == myth_templar_enemy
or is_qmark($b) == templar_absolution
        if is_qmark($b) == templar_absolution 100005
                at $b echoat $b [1;35m±Ð·|Å«¸o¨é                   §¹¦¨[0m
        else
                at $b echoat $b [1;32m±Ð·|Å«¸o¨é                   ¥¼§¹[0m
        endif
endif
at $b echoat $b .             [1;37m{·t¶ÂºëÆF»â¦a}[0m
if is_qmark($b) == enclave_quest 100007
        at $b echoat $b [1;35m´M§ä¤H½è                     §¹¦¨[0m
else
        at $b echoat $b [1;32m´M§ä¤H½è                     ¥¼§¹[0m
endif
def_rembch null-tar
~
>room_defun boss_list~
at $b echoat $b [1;37m§A¥i¥H¬d¬Ýªº¥Ø¼Ðª«°Ï°ì:[0m
at $b echoat $b ²Ä¤@­¶ [1;32m¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì I           [37m(    boss01     )[0m
at $b echoat $b ²Ä¤G­¶ [1;32m¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì II          [37m(    boss02     )[0m
at $b echoat $b ²Ä¤T­¶ ªÅ­¶
at $b echoat $b ²Ä¥|­¶ [1;32m¥v¥d©Ô¨©¦èÃä°Ï°ì I             [37m(    boss04     )[0m
at $b echoat $b ²Ä¤­­¶ [1;32m¥v¥d©Ô¨©¦èÃä°Ï°ì II            [37m(    boss05     )[0m
at $b echoat $b ²Ä¤»­¶ ªÅ­¶
at $b echoat $b ²Ä¤C­¶ [1;32m¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï I   [37m(    boss07     )[0m
at $b echoat $b ²Ä¤K­¶ [1;32m¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï II  [37m(    boss08     )[0m
at $b echoat $b ²Ä¤E­¶ ªÅ­¶
at $b echoat $b ²Ä¤Q­¶ [1;32m¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï I         [37m(    boss10     )[0m
at $b echoat $b ²Ä11­¶ [1;32m¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï II        [37m(    boss11     )[0m
at $b echoat $b ²Ä12­¶ ªÅ­¶
at $b echoat $b ²Ä13­¶ [1;36m°gÃú¤§®q¥D°Ï                   [37m(    boss13     )[0m
at $b echoat $b ²Ä14­¶ ªÅ­¶
at $b echoat $b ²Ä15­¶ ªÅ­¶
at $b echoat $b ²Ä16­¶ [1;36m°gÃú¤§®q©`¹Ï¤ý°ê               [37m(    boss16     )[0m
at $b echoat $b ²Ä17­¶ ªÅ­¶
at $b echoat $b ²Ä18­¶ ªÅ­¶
at $b echoat $b ²Ä19­¶ [1;36m°gÃú¤§®q¤§¨ä¥L°Ï°ì             [37m(    boss19     )[0m
at $b echoat $b ²Ä20­¶ ªÅ­¶
at $b echoat $b ²Ä21­¶ ªÅ­¶
at $b echoat $b ²Ä22­¶ [1;33m»ÈÀs®üÆWº[¦a¼hÅ]°ì             [37m(    boss22     )[0m
def_rembch null-tar
~
>room_defun b01~
at $b echoat $b .		[1;36m[¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì I][0m
if is_qmark($b) & adv_book_1_main 100001
	at $b echoat $b [1;35m°õªkªÌ                  ¥v¥d©Ô¨©       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m°õªkªÌ                  ¥v¥d©Ô¨©       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100002
	at $b echoat $b [1;35m²×µ²ªÌ                  ¥v¥d©Ô¨©       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m²×µ²ªÌ                  ¥v¥d©Ô¨©       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100004
	at $b echoat $b [1;35mÂL©g¶®                  ¥v¥d©Ô¨©       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÂL©g¶®                  ¥v¥d©Ô¨©       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100008
	at $b echoat $b [1;35mÄõ´µ¬¥¯S                ¥v¥d©Ô¨©       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÄõ´µ¬¥¯S                ¥v¥d©Ô¨©       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b02~
at $b echoat $b .		[1;36m[¥v¥d©Ô¨©¤Î¶gÃä°Ï°ì II][0m
if is_qmark($b) & adv_book_1_main 200001
	at $b echoat $b [1;35m¤p´c°­¥~³ò­x¹Îªø        ±Kµn¥§´ËªL     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤p´c°­¥~³ò­x¹Îªø        ±Kµn¥§´ËªL     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200002
	at $b echoat $b [1;35m¤p´c°­¸T½Ã­x¹Îªø        ±Kµn¥§´ËªL     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤p´c°­¸T½Ã­x¹Îªø        ±Kµn¥§´ËªL     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200004
	at $b echoat $b [1;35m¤p´c°­Á`°Ñ¿Ñ¼Ú¤Ú®á      ±Kµn¥§´ËªL     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤p´c°­Á`°Ñ¿Ñ¼Ú¤Ú®á      ±Kµn¥§´ËªL     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200256
	at $b echoat $b [1;35m¤p´c°­©¸¯¥              ±Kµn¥§´ËªL     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤p´c°­©¸¯¥              ±Kµn¥§´ËªL     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200512
	at $b echoat $b [1;35m¤p´c°­§ð«°ºl            ±Kµn¥§´ËªL     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤p´c°­§ð«°ºl            ±Kµn¥§´ËªL     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 201024
	at $b echoat $b [1;35m¤p´c°­§ë¥Û¾÷            ±Kµn¥§´ËªL     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤p´c°­§ë¥Û¾÷            ±Kµn¥§´ËªL     ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b03~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b04~
at $b echoat $b .		[1;36m[¥v¥d©Ô¨©¦èÃä°Ï°ì I][0m
if is_qmark($b) & adv_book_1_main 100016
	at $b echoat $b [1;35m¶Àª÷¼C«È                »È³DªZ¤h³ù     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶Àª÷¼C«È                »È³DªZ¤h³ù     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100032
	at $b echoat $b [1;35m»È³D³ù³ù¥D              »È³DªZ¤h³ù     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m»È³D³ù³ù¥D              »È³DªZ¤h³ù     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100064
	at $b echoat $b [1;35m¼C«Èºû¹F                »È³DªZ¤h³ù     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¼C«Èºû¹F                »È³DªZ¤h³ù     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200032
	at $b echoat $b [1;35m¥¨Å]»â¾ÉªÌ¼Þ©Ô¼Þ©Ô      ¥jªh¦a         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥¨Å]»â¾ÉªÌ¼Þ©Ô¼Þ©Ô      ¥jªh¦a         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100128
	at $b echoat $b [1;35m·ç­s»¨§J                ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m·ç­s»¨§J                ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100256
	at $b echoat $b [1;35mºñ¾ð½Ã¶¤¶¤ªø            ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mºñ¾ð½Ã¶¤¶¤ªø            ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 100512
	at $b echoat $b [1;35m±öÂL©Ô                  ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m±öÂL©Ô                  ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 101024
	at $b echoat $b [1;35m¥d¬î±þ                  ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥d¬î±þ                  ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 102048
	at $b echoat $b [1;35mºñ¾ð½Ã¶¤°Æ¶¤ªø          ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mºñ¾ð½Ã¶¤°Æ¶¤ªø          ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 104096
	at $b echoat $b [1;35m°h¥ð¤j¦Ú                ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m°h¥ð¤j¦Ú                ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 108192
	at $b echoat $b [1;35m¸ëÅQ                    ºñ¾ð¤ý°ê       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¸ëÅQ                    ºñ¾ð¤ý°ê       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 200001
	at $b echoat $b [1;35m½{¶®º¿¯S                Às¶ð           ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m½{¶®º¿¯S                Às¶ð           ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 200002
	at $b echoat $b [1;35mÅ]ªk¤k¤ý                Å]ªk°ª¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÅ]ªk¤k¤ý                Å]ªk°ª¶ð       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b05~
at $b echoat $b .		[1;36m[¥v¥d©Ô¨©¦èÃä°Ï°ì II][0m
if is_qmark($b) & adv_book_1_west 100001
	at $b echoat $b [1;35mÃ¹­J¤½Àï                Ã¹­J«°³ù       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÃ¹­J¤½Àï                Ã¹­J«°³ù       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100002
	at $b echoat $b [1;35mÃ¹­J¤Ò¤H                Ã¹­J«°³ù       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÃ¹­J¤Ò¤H                Ã¹­J«°³ù       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100004
	at $b echoat $b [1;35mÅÜºØ¥ÕÀs                Ã¹­J«°³ù       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÅÜºØ¥ÕÀs                Ã¹­J«°³ù       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100008
	at $b echoat $b [1;35m«Õ­ß¥¨Ã~                «Õ­ß²`²W       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m«Õ­ß¥¨Ã~                «Õ­ß²`²W       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100016
	at $b echoat $b [1;35mîî¼v´cÅ]1               «Õ­ß²`²W       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mîî¼v´cÅ]1               «Õ­ß²`²W       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100032
	at $b echoat $b [1;35mîî¼v´cÅ]2               «Õ­ß²`²W       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mîî¼v´cÅ]2               «Õ­ß²`²W       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100064
	at $b echoat $b [1;35mîî¼v¤§¤ý                «Õ­ß²`²W       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mîî¼v¤§¤ý                «Õ­ß²`²W       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 116384
	at $b echoat $b [1;35m¥j¥N­¸Às                ¸TÂ_¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥j¥N­¸Às                ¸TÂ_¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 132768
	at $b echoat $b [1;35mîî¶Â¤§ÆF                ¸TÂ_¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mîî¶Â¤§ÆF                ¸TÂ_¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 200004
	at $b echoat $b [1;35m´cÅ]ÃM¤h                ¸TÂ_¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m´cÅ]ÃM¤h                ¸TÂ_¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100128
	at $b echoat $b [1;35m¶ÂÅ]Às                  ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶ÂÅ]Às                  ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100256
	at $b echoat $b [1;35m¼Ú­{                    ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¼Ú­{                    ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 100512
	at $b echoat $b [1;35m¼Úµ·                    ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¼Úµ·                    ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 101024
	at $b echoat $b [1;35m§J¨½¯S                  ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m§J¨½¯S                  ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 102048
	at $b echoat $b [1;35m½{´µ¡D®R                ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m½{´µ¡D®R                ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 104096
	at $b echoat $b [1;35mªk°ö                    ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mªk°ö                    ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_west 108192
	at $b echoat $b [1;35m¶Â¤ô´¹Às³ÈÀw            ª¢¬õ¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶Â¤ô´¹Às³ÈÀw            ª¢¬õ¤§¶ð       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b06~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b07~
at $b echoat $b .		[1;36m[¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï I][0m
if is_qmark($b) & adv_book_1_main 200008
	at $b echoat $b [1;35m¥j¦«¨¹                  ºëÆF¤§´Ë       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥j¦«¨¹                  ºëÆF¤§´Ë       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200016
	at $b echoat $b [1;35m¥j©Ô¨¹                  ºëÆF¤§´Ë       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥j©Ô¨¹                  ºëÆF¤§´Ë       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200064
	at $b echoat $b [1;35m¥Ì¹D¤Ò                  ÁÉ¥ï´µ©ì´¶     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥Ì¹D¤Ò                  ÁÉ¥ï´µ©ì´¶     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100016
	at $b echoat $b [1;35m²ö´µ´£º¿                ®õ¬¥´µ         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m²ö´µ´£º¿                ®õ¬¥´µ         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100032
	at $b echoat $b [1;35mº¿ªù                    ®õ¬¥´µ         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mº¿ªù                    ®õ¬¥´µ         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100064
	at $b echoat $b [1;35m¶ÂÅì¨¸Às»®·çªR          ®õ¬¥´µ         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶ÂÅì¨¸Às»®·çªR          ®õ¬¥´µ         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100128
	at $b echoat $b [1;35m«Í§¯µÜ¿Õ                ®õ¬¥´µ         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m«Í§¯µÜ¿Õ                ®õ¬¥´µ         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100256
	at $b echoat $b [1;35m«C»É¥¨¤H¶ð¬¥´µ          ®õ¬¥´µ         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m«C»É¥¨¤H¶ð¬¥´µ          ®õ¬¥´µ         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100512
	at $b echoat $b [1;35m¨È¼Ü¬}¥Þ¥¨¤H            ²`¤g«°         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¨È¼Ü¬}¥Þ¥¨¤H            ²`¤g«°         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100008
	at $b echoat $b [1;35m¬Ó¤kªü¨fÁ¢              ²`¤g«°         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¬Ó¤kªü¨fÁ¢              ²`¤g«°         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100016
	at $b echoat $b [1;35m©Ô®u¥d                  ²`¤g«°         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m©Ô®u¥d                  ²`¤g«°         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100032
	at $b echoat $b [1;35m¯Áº¸®æªL©i              ²`¤g«°         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¯Áº¸®æªL©i              ²`¤g«°         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100064
	at $b echoat $b [1;35m±öªk©Ô                  ²`¤g«°         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m±öªk©Ô                  ²`¤g«°         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100128
	at $b echoat $b [1;35m²V¨P¤EÀY³D              ²`¤gªh¿A       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m²V¨P¤EÀY³D              ²`¤gªh¿A       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100256
	at $b echoat $b [1;35m¸U¥j¾ð§¯                ²`¤gªh¿A       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¸U¥j¾ð§¯                ²`¤gªh¿A       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 101024
	at $b echoat $b [1;35mÂù¨¬Às¤ý                ²`¤gªh¿A       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÂù¨¬Às¤ý                ²`¤gªh¿A       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b08~
at $b echoat $b .		[1;36m[¥v¥d©Ô¨©ªFÃä°Ï°ì¤Î¤j¨Fºz°Ï II][0m
if is_qmark($b) & adv_book_1_east 200002
	at $b echoat $b [1;35m¤j´cÅ]                  ¥Ã«í¤§¶ð       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤j´cÅ]                  ¥Ã«í¤§¶ð       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100001
	at $b echoat $b [1;35m¶®§J                    ÆZ¯î´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶®§J                    ÆZ¯î´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100002
	at $b echoat $b [1;35m¦ã½¬®R                  ÆZ¯î´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦ã½¬®R                  ÆZ¯î´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 100004
	at $b echoat $b [1;35mÁÉ¤Ú¬¥¥q                ÆZ¯î´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÁÉ¤Ú¬¥¥q                ÆZ¯î´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_east 200001
	at $b echoat $b [1;35m¦ã²ú¤kÀï                ³Á®æ«°         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦ã²ú¤kÀï                ³Á®æ«°         ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b09~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b10~
at $b echoat $b .		[1;36m[¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï I][0m
if is_qmark($b) & adv_book_1_other 100001
	at $b echoat $b [1;35m¤j¤£¦CÄA­^¤ý            ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤j¤£¦CÄA­^¤ý            ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100002
	at $b echoat $b [1;35m¸tªÌ                    ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¸tªÌ                    ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100004
	at $b echoat $b [1;35m®É¥ú¤§¯«                ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m®É¥ú¤§¯«                ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100008
	at $b echoat $b [1;35m»X¤¦                    ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m»X¤¦                    ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100016
	at $b echoat $b [1;35m¦Ì®R                    ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦Ì®R                    ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100032
	at $b echoat $b [1;35m®J¯Á¼w´µ                ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m®J¯Á¼w´µ                ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100064
	at $b echoat $b [1;35m·t¶Â¤§¤ý                ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m·t¶Â¤§¤ý                ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 100128
	at $b echoat $b [1;35m¦uÅ@ªÌ                  ³Ð¥@¬ö         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦uÅ@ªÌ                  ³Ð¥@¬ö         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 200128
	at $b echoat $b [1;35m¶Â«Ò´µ                  ¶øªL©¬´µ¤s     ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶Â«Ò´µ                  ¶øªL©¬´µ¤s     ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 116384
	at $b echoat $b [1;35m¦N´µ¤ý                  ¬P¬É           ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦N´µ¤ý                  ¬P¬É           ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_main 132768
	at $b echoat $b [1;35m©¬©Ôº¿´µ                ¯I½L¤j¦a       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m©¬©Ôº¿´µ                ¯I½L¤j¦a       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b11~
at $b echoat $b .		[1;36m[¥­¦æ¥@¬É¥H¤Î¥~³ò¦a°Ï I][0m
if is_qmark($b) & adv_book_1_other 200001
	at $b echoat $b [1;35m³·¦Z                    «n·¥¤j¦a       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m³·¦Z                    «n·¥¤j¦a       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_1_other 200002
	at $b echoat $b [1;35m¬h·q©ú                  ²MªeÂí         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¬h·q©ú                  ²MªeÂí         ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b12~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b13~
at $b echoat $b .		[1;36m[°gÃú¤§®q¥D°Ï][0m
if is_qmark($b) & adv_book_2_1 100032
	at $b echoat $b [1;35m¥dº¿´µ¡@                ·¬¬õªL         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥dº¿´µ¡@                ·¬¬õªL         ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b14~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b15~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b16~
at $b echoat $b .		[1;36m[°gÃú¤§®q©`¹Ï¤ý°ê][0m
if is_qmark($b) & adv_book_2_1 100064
	at $b echoat $b [1;35m¯«¸t±O±þªÌ              °ª¦a¡@         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¯«¸t±O±þªÌ              °ª¦a¡@         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100128
	at $b echoat $b [1;35m§l¦å°­¤ý¡@¡@            °­Âí¡@¡@       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m§l¦å°­¤ý¡@¡@            °­Âí¡@¡@       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100256
	at $b echoat $b [1;35m¦ºÆF¤§¤ý¡@¡@            °­Âí¡@¡@       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦ºÆF¤§¤ý¡@¡@            °­Âí¡@¡@       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100512
	at $b echoat $b [1;35m¤ýªÌ´cÅ]                °­Âí¡@¡@       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤ýªÌ´cÅ]                °­Âí¡@¡@       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 101024
	at $b echoat $b [1;35m¶Â·t¤§¤ý¡@¡@            °­Âí¡@¡@       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶Â·t¤§¤ý¡@¡@            °­Âí¡@¡@       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 102048
	at $b echoat $b [1;35m¼Z¤Ñ¨Ïªø¡@              °­Âí¡@¡@       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¼Z¤Ñ¨Ïªø¡@              °­Âí¡@¡@       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 104096
	at $b echoat $b [1;35m¦æ¼vªÌ¡@                °­Âí¡@¡@       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦æ¼vªÌ¡@                °­Âí¡@         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 108192
	at $b echoat $b [1;35m´HÁ÷´cÅ]¡@              °­Âí¡@         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m´HÁ÷´cÅ]¡@              °­Âí¡@         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 200001
	at $b echoat $b [1;35m·Òº»´cÅ]                °­Âí¡@         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m·Òº»´cÅ]                °­Âí¡@         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 200002
	at $b echoat $b [1;35m¥|Ál¼Z¤Ñ¨Ï              °­Âí¡@         ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥|Ál¼Z¤Ñ¨Ï              °­Âí¡@         ¥¼À»±Ñ[0m
endif
if is_qmark($b) & wander_quest 200001
	at $b echoat $b [1;35m§Q¤ú±þ¤â¡@¡@            ¤Û´b°gªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m§Q¤ú±þ¤â¡@¡@            ¤Û´b°gªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & wander_quest 200004
	at $b echoat $b [1;35m²r¬r«r¾½¡@¡@            ¤Û´b°gªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m²r¬r«r¾½¡@¡@            ¤Û´b°gªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & wander_quest 200016
	at $b echoat $b [1;35m¤ýªÌ¤§«a                ¤Û´b°gªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤ýªÌ¤§«a                ¤Û´b°gªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & wander_quest 200064
	at $b echoat $b [1;35m¼ÉÄ¼¥~¥Ö¡@¡@            ¤Û´b°gªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¼ÉÄ¼¥~¥Ö¡@¡@            ¤Û´b°gªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & wander_quest 200256
	at $b echoat $b [1;35m¦å¸{¤§®±¡@              ¤Û´b°gªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦å¸{¤§®±¡@              ¤Û´b°gªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & wander_quest 201024
	at $b echoat $b [1;35m¶Àª÷²¯¥Ò¡@              ¤Û´b°gªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶Àª÷²¯¥Ò¡@              ¤Û´b°gªL       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b17~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b18~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b19~
at $b echoat $b .		[1;36m[°gÃú¤§®q¤§¨ä¥L°Ï°ì][0m
if is_qmark($b) & adv_book_2_1 100001
	at $b echoat $b [1;35m»·¥j´cÅ]                »·¥j´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m»·¥j´cÅ]                »·¥j´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100002
	at $b echoat $b [1;35m®õ©Z¥¨¤H­º»â            »·¥j´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m®õ©Z¥¨¤H­º»â            »·¥j´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100004
	at $b echoat $b [1;35m±ö§ù²ï  ¡@¡@            »·¥j´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m±ö§ù²ï  ¡@¡@            »·¥j´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100008
	at $b echoat $b [1;35m»·¥j¥½¤é¥¨Ã~            »·¥j´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m»·¥j¥½¤é¥¨Ã~            »·¥j´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & adv_book_2_1 100016
	at $b echoat $b [1;35m¥¨«¬·àølÃ~¡@            »·¥j´ËªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥¨«¬·àølÃ~¡@            »·¥j´ËªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & sealevil_quest 300001
	at $b echoat $b [1;35m¥§°Ç¡@¡@¡@¡@            «ÊÅ]¬}¸]       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¥§°Ç¡@¡@¡@¡@            «ÊÅ]¬}¸]       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & sealevil_quest 300002
	at $b echoat $b [1;35m²öÃ¹¤ñ´µ¡@¡@            «ÊÅ]¬}¸]       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m²öÃ¹¤ñ´µ¡@¡@            «ÊÅ]¬}¸]       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & sealevil_quest 300004
	at $b echoat $b [1;35m¦ã±ö¸¦¡@                «ÊÅ]¬}¸]       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¦ã±ö¸¦¡@                «ÊÅ]¬}¸]       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & sealevil_quest 300008
	at $b echoat $b [1;35m¶øªk¡@¡@¡@¡@            «ÊÅ]¬}¸]       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶øªk¡@¡@¡@¡@            «ÊÅ]¬}¸]       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & sealevil_quest 300016
	at $b echoat $b [1;35m­}·çº¸¡@¡@              «ÊÅ]¬}¸]       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m­}·çº¸¡@¡@              «ÊÅ]¬}¸]       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
>room_defun b20~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b21~
at $b echoat $b ªÅ­¶
def_rembch null-tar
~
>room_defun b22~
at $b echoat $b .		[1;36m[»ÈÀs®üÆWº[¦a¼hÅ]°ì][0m
if is_qmark($b) & dragonbay_quest_list 100008
	at $b echoat $b [1;35m¨È¼Ü¥¨¤H¤ý¡@            ¦a¼hÅ]°ì       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¨È¼Ü¥¨¤H¤ý¡@            ¦a¼hÅ]°ì       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & dragonbay_quest_list 100016
	at $b echoat $b [1;35mÅ]²´»jµï¬Ó¦Z            Å]²´ÂOªL       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32mÅ]²´»jµï¬Ó¦Z            Å]²´ÂOªL       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & dragonbay_quest_list 100032
	at $b echoat $b [1;35m¤ûÅ]¤ý        ¡@        ¦a¼hÅ]°ì       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤ûÅ]¤ý        ¡@        ¦a¼hÅ]°ì       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & dragonbay_quest_list 100064
	at $b echoat $b [1;35m¶Ý¦å±N­x¡@¡@            ¦a¼hÅ]°ì       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¶Ý¦å±N­x¡@¡@            ¦a¼hÅ]°ì       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & dragonbay_quest_list 100128
	at $b echoat $b [1;35m¸U¼v¤§¤ý¡@              ¦a¼hÅ]°ì       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¸U¼v¤§¤ý¡@              ¦a¼hÅ]°ì       ¥¼À»±Ñ[0m
endif
if is_qmark($b) & dragonbay_quest_list 100256
	at $b echoat $b [1;35m¤k§¯¤§¦Z¡@              ¦a¼hÅ]°ì       ¤wÀ»±Ñ[0m
else
	at $b echoat $b [1;32m¤k§¯¤§¦Z¡@              ¦a¼hÅ]°ì       ¥¼À»±Ñ[0m
endif
def_rembch null-tar
~
|
