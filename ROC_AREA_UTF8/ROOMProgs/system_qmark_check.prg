>room_defun quest_list~
at $b echoat $b [1;37m你可以查看的任務區域:[0m
at $b echoat $b 第零頁 [1;33m銀龍海灣暨地層魔域             [37m(      00       )[0m
at $b echoat $b 第一頁 [1;32m史卡拉貝及週邊區域 I           [37m(      01       )[0m
at $b echoat $b 第二頁 [1;32m史卡拉貝及週邊區域 II          [37m(      02       )[0m
at $b echoat $b 第三頁 [1;32m史卡拉貝西邊區域 I             [37m(      03       )[0m
at $b echoat $b 第四頁 [1;32m史卡拉貝西邊區域 II            [37m(      04       )[0m
at $b echoat $b 第五頁 [1;32m史卡拉貝東邊區域及大沙漠區 I   [37m(      05       )[0m
at $b echoat $b 第六頁 [1;32m史卡拉貝東邊區域及大沙漠區 II  [37m(      06       )[0m
at $b echoat $b 第七頁 [1;32m平行世界以及外圍地區 I         [37m(      07       )[0m
at $b echoat $b 第八頁 [1;32m平行世界以及外圍地區 II        [37m(      08       )[0m
at $b echoat $b 第九頁 [1;36m迷霧之島主區                   [37m(      09       )[0m
at $b echoat $b 第十頁 [1;36m迷霧之島奈圖王國               [37m(      10       )[0m
at $b echoat $b 第11頁 [1;36m迷霧之島之其他區域 I           [37m(      11       )[0m
at $b echoat $b 第12頁 [1;36m迷霧之島之其他區域 II          [37m(      12       )[0m
at $b echoat $b 第13頁 [1;36m迷霧之島之其他區域 III         [37m(      13       )[0m
def_rembch null-tar
~
>room_defun q00~
at $b echoat $b .             [1;36m[銀龍海灣暨地層魔域][0m
at $b echoat $b .             [1;37m{銀龍海灣}[0m
if is_qmark($b) == dragonbay_mayor_secret 100010
or is_qmark($b) & dragonbay_quest_mark 100004
        at $b echoat $b [1;35m神選之人                     完成[0m
else
        at $b echoat $b [1;32m神選之人                     未完[0m
endif
at $b echoat $b .             [1;37m{魔眼叢林}[0m
if is_qmark($b) & dragonbay_quest_mark 200001
        at $b echoat $b [1;35m大長老的希望                 完成[0m
else
        at $b echoat $b [1;32m大長老的希望                 未完[0m
endif
if is_qmark($b) & dragonbay_quest_mark 200002
        at $b echoat $b [1;35m燃燒                         完成[0m
else
        at $b echoat $b [1;32m燃燒                         未完[0m
endif
if is_qmark($b) & dragonbay_quest_mark 200004
        at $b echoat $b [1;35m資源回收                     完成[0m
else
        at $b echoat $b [1;32m資源回收                     未完[0m
endif
def_rembch null-tar
~
>room_defun q01~
at $b echoat $b .             [1;36m[史卡拉貝及週邊區域 I][0m
at $b echoat $b .             [1;37m{史卡拉貝城}[0m
if is_qmark($b) == skara_baby_quest1 100005
        at $b echoat $b [1;35m老者的託付                   完成[0m
else
        at $b echoat $b [1;32m老者的託付                   未完[0m
endif
if is_qmark($b) == skara_baby_quest2 100003
        at $b echoat $b [1;35m醫者的龍葵                   完成[0m
else
        at $b echoat $b [1;32m醫者的龍葵                   未完[0m
endif
if is_qmark($b) == skara_baby_quest3 100003
        at $b echoat $b [1;35m團隊與合作                   完成[0m
else
        at $b echoat $b [1;32m團隊與合作                   未完[0m
endif
if is_qmark($b) == midennir_lostboy_exp
        at $b echoat $b [1;35m焦慮的母親                   完成[0m
else
        at $b echoat $b [1;32m焦慮的母親                   未完[0m
endif
if primeattr($b) == 2
or class($b) == 2
or class($b) == 9
or class($b) == 10
or class($b) == 17
        at $b echoat $b [1;30m(秘)[37m盜賊公會任務:[0m
if is_qmark($b) !& thief_quest_list 100001
        at $b echoat $b [1;35m--秘密調查                   完成[0m
else
        at $b echoat $b [1;32m--秘密調查                   未完[0m
endif
if is_qmark($b) !& thief_quest_list 100002
        at $b echoat $b [1;35m--妙手精精兒                 完成[0m
else
        at $b echoat $b [1;32m--妙手精精兒                 未完[0m
endif
if is_qmark($b) !& thief_quest_list 100008
        at $b echoat $b [1;35m--妙手空空兒                 完成[0m
else
        at $b echoat $b [1;32m--妙手空空兒                 未完[0m
endif
if is_qmark($b) !& thief_quest_list 100004
        at $b echoat $b [1;35m--暗通情款                   完成[0m
else
        at $b echoat $b [1;32m--暗通情款                   未完[0m
endif
if is_qmark($b) !& thief_quest_list 100032
        at $b echoat $b [1;35m--劫獄                       完成[0m
else
        at $b echoat $b [1;32m--劫獄                       未完[0m
endif
if is_qmark($b) !& thief_quest_list 100016
        at $b echoat $b [1;35m--暗殺                       完成[0m
else
        at $b echoat $b [1;32m--暗殺                       未完[0m
endif
if is_qmark($b) !& thief_quest_list 100064
        at $b echoat $b [1;35m--血腥暗殺                   完成[0m
else
        at $b echoat $b [1;32m--血腥暗殺                   未完[0m
endif
if is_qmark($b) !& thief_quest_list 100128
        at $b echoat $b [1;35m--空手入白刃                 完成[0m
else
        at $b echoat $b [1;32m--空手入白刃                 未完[0m
endif
if is_qmark($b) == thief_quest_reward 100001
        at $b echoat $b [1;35m--無等級暗器鞋               完成[0m
else
        at $b echoat $b [1;32m--無等級暗器鞋               未完[0m
endif
endif
if primeattr($b) == 3
or class($b) == 0
or class($b) == 5
or class($b) == 6
or class($b) == 15
        at $b echoat $b [1;30m(秘)[37m法師公會任務:[0m
if is_qmark($b) == mage_envoy_quest 100024
        at $b echoat $b [1;35m--光榮密使                   完成[0m
else
        at $b echoat $b [1;32m--光榮密使                   未完[0m
endif
endif
if is_qmark($b) == enable_level_40
        at $b echoat $b [1;35m勇者稱號                     完成[0m
else
        at $b echoat $b [1;32m勇者稱號                     未完[0m
endif
if is_qmark($b) == enable_level_41
        at $b echoat $b [1;35m英雄稱號                     完成[0m
else
        at $b echoat $b [1;32m英雄稱號                     未完[0m
endif
if is_qmark($b) == skarabrae_joe_quest 100004
        at $b echoat $b [1;35m喬藍多的煩惱                 完成[0m
else
        at $b echoat $b [1;32m喬藍多的煩惱                 未完[0m
endif
if is_qmark($b) == commerce_crystal_quest 100006
        at $b echoat $b [1;35m珠寶檢定                     完成[0m
else
        at $b echoat $b [1;32m珠寶檢定                     未完[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100001
        at $b echoat $b [1;35m鬧鬼的書本                   完成[0m
else
        at $b echoat $b [1;32m鬧鬼的書本                   未完[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100002
        at $b echoat $b [1;35m各懷鬼胎的委託               完成[0m
else
        at $b echoat $b [1;32m各懷鬼胎的委託               未完[0m
endif
if is_qmark($b) & skarabrae_study_questmark 100004
        at $b echoat $b [1;35m保密防諜                     完成[0m
else
        at $b echoat $b [1;32m保密防諜                     未完[0m
endif
if is_qmark($b) == valentine_quest 300511
or is_qmark($b) == valentine_quest 301023
        at $b echoat $b [1;35m愛的禮物                     完成[0m
else
        at $b echoat $b [1;32m愛的禮物                     未完[0m
endif
if is_qmark($b) != commerce_enemy
at $b echoat $b .             [1;37m{史卡拉貝商會}[0m
if is_qmark($b) == adv_guild_level
	at $b echoat $b [1;30m冒險者公會評價:[0m
	if is_qmark($b) & adv_guild_level 300004
		at $b echoat $b [1;35m--冷門委託愛好者             完成[0m
	else
		if is_qmark($b) & adv_guild_level 300002
			at $b echoat $b [1;35m--雜事能手                   完成[0m
		else
			if is_qmark($b) & adv_guild_level 300001
				at $b echoat $b [1;35m--公會新參者                 完成[0m
			endif
		endif
	endif
endif
if is_qmark($b) == commerce_noob_mage 300001
or is_qmark($b) == commerce_noob_mage 300002
or is_qmark($b) & commerce_subquest 100001
or is_qmark($b) & commerce_subquest 200001
		at $b echoat $b [1;35m初出茅廬的法師               完成[0m
else
		at $b echoat $b [1;32m初出茅廬的法師               未完[0m
endif
if is_qmark($b) == scar_adv_helping 300001
or is_qmark($b) == scar_adv_helping 300002
or is_qmark($b) & commerce_subquest 100002
or is_qmark($b) & commerce_subquest 200002
		at $b echoat $b [1;35m冒險者的仁義                 完成[0m
else
		at $b echoat $b [1;32m冒險者的仁義                 未完[0m
endif
if is_qmark($b) & commerce_subquest 100004
		at $b echoat $b [1;35m地下交易廳                   完成[0m
endif
if is_qmark($b) & commerce_subquest 100008
		at $b echoat $b [1;35m曼先的祖產                   完成[0m
endif
if is_qmark($b) == pharmacist_level
	if is_qmark($b) & pharmacist_level 100004
		at $b echoat $b [1;30m(秘)[0m[1;35m藥品鍊成 I               完成[0m
	else
		at $b echoat $b [1;30m(秘)[0m[1;32m藥品鍊成 I               未完[0m
	endif
endif
endif
def_rembch null-tar
~
>room_defun q02~
at $b echoat $b .             [1;36m[史卡拉貝及週邊區域 II][0m
at $b echoat $b .             [1;37m{小精靈村}[0m
if is_qmark($b) == smurf_save 100002
        at $b echoat $b [1;35m替小精靈出口氣               完成[0m
else
        at $b echoat $b [1;32m替小精靈出口氣               未完[0m
endif
at $b echoat $b .             [1;37m{馬戲團}[0m
if is_qmark($b) & circus_quest 100001
        at $b echoat $b [1;35m解救工作人員                 完成[0m
else
        at $b echoat $b [1;32m解救工作人員                 未完[0m
endif
if is_qmark($b) & circus_quest 100002
        at $b echoat $b [1;35m小鮑比的媽媽                 完成[0m
else
        at $b echoat $b [1;32m小鮑比的媽媽                 未完[0m
endif
at $b echoat $b .             [1;37m{密登尼森林}[0m
if is_qmark($b) == midennir_bard_quest 100002
        at $b echoat $b [1;35m吟遊詩人之歌                 完成[0m
else
        at $b echoat $b [1;32m吟遊詩人之歌                 未完[0m
endif
if is_qmark($b) >= midennir_quest 100001
        at $b echoat $b [1;35m直擣哥布林總部               完成[0m
else
        at $b echoat $b [1;32m直擣哥布林總部               未完[0m
endif
at $b echoat $b .             [1;37m{巢穴貧民區}[0m
if is_qmark($b) & slum_libei_quest 100001
        at $b echoat $b [1;35m打零工                       完成[0m
else
        at $b echoat $b [1;32m打零工                       未完[0m
endif
at $b echoat $b .             [1;37m{鼠人巢穴}[0m
if is_qmark($b) == wererat_save 100002
        at $b echoat $b [1;35m可憐的囚犯                   完成[0m
else
        at $b echoat $b [1;32m可憐的囚犯                   未完[0m
endif
at $b echoat $b .             [1;37m{北方平原}[0m
if is_qmark($b) == plains_slaver_quest 200004
        at $b echoat $b [1;35m奴隸販子                     完成[0m
else
        at $b echoat $b [1;32m奴隸販子                     未完[0m
endif
at $b echoat $b .             [1;37m{教堂墓窖}[0m
if is_qmark($b) == chapel_question 100004
        at $b echoat $b [1;35m墓園之謎                     完成[0m
else
        at $b echoat $b [1;32m墓園之謎                     未完[0m
endif
at $b echoat $b .             [1;37m{邪龍祭壇}[0m
if is_qmark($b) == cult_question_end
        at $b echoat $b [1;35m拯救夏洛蒂                   完成[0m
else
        at $b echoat $b [1;32m拯救夏洛蒂                   未完[0m
endif
if is_qmark($b) == cult_rock_end
        at $b echoat $b [1;35m大地之石                     完成[0m
else
        at $b echoat $b [1;32m大地之石                     未完[0m
endif
at $b echoat $b .             [1;37m{地精村}[0m
if is_qmark($b) & gnome_quest 100002
        at $b echoat $b [1;35m族長的煩惱                   完成[0m
else
        at $b echoat $b [1;32m族長的煩惱                   未完[0m
endif
if is_qmark($b) & gnome_quest 100004
        at $b echoat $b [1;35m大惡鬼紛爭                   完成[0m
else
        at $b echoat $b [1;32m大惡鬼紛爭                   未完[0m
endif
at $b echoat $b .             [1;37m{叛亂軍城寨}[0m
if is_qmark($b) > rebel_questmark 100004
        at $b echoat $b [1;35m斬了那個玩屍體的傢伙！       完成[0m
else
        at $b echoat $b [1;32m斬了那個玩屍體的傢伙！       未完[0m
endif
if is_qmark($b) > rebel_questmark 100017
        at $b echoat $b [1;35m就地取材                     完成[0m
else
        at $b echoat $b [1;32m就地取材                     未完[0m
endif
if is_qmark($b) > rebel_questmark 100021
        at $b echoat $b [1;35m就地取材‧其之二             完成[0m
else
        at $b echoat $b [1;32m就地取材‧其之二             未完[0m
endif
if is_qmark($b) > rebel_questmark 100029
        at $b echoat $b [1;35m搶錢、搶糧、搶......         完成[0m
else
        at $b echoat $b [1;32m搶錢、搶糧、搶......         未完[0m
endif
if is_qmark($b) > rebel_questmark 100034
        at $b echoat $b [1;35m燃燒吧，鬥志                 完成[0m
else
        at $b echoat $b [1;32m燃燒吧，鬥志                 未完[0m
endif
if is_qmark($b) > rebel_questmark 100038
        at $b echoat $b [1;35m炸彈魔覺醒之日               完成[0m
else
        at $b echoat $b [1;32m炸彈魔覺醒之日               未完[0m
endif
if is_qmark($b) > rebel_questmark 100042
        at $b echoat $b [1;35m破壞時刻                     完成[0m
else
        at $b echoat $b [1;32m破壞時刻                     未完[0m
endif
if is_qmark($b) > rebel_questmark 100048
        at $b echoat $b [1;35m靠嘴巴還是靠拳頭？           完成[0m
else
        at $b echoat $b [1;32m靠嘴巴還是靠拳頭？           未完[0m
endif
if is_qmark($b) > rebel_questmark 100055
        at $b echoat $b [1;35m最後的抵抗                   完成[0m
else
        at $b echoat $b [1;32m最後的抵抗                   未完[0m
endif
if is_qmark($b) > rebel_questmark 100059
        at $b echoat $b [1;35m討逆戰爭落幕                 完成[0m
else
        at $b echoat $b [1;32m討逆戰爭落幕                 未完[0m
endif
if is_qmark($b) == rebel_questmark 100066
        at $b echoat $b [1;30m(秘)[0m[1;35m臨終的精靈與黑暗娃娃     完成[0m
endif
def_rembch null-tar
~
>room_defun q03~
at $b echoat $b .             [1;36m[史卡拉貝西邊區域 I][0m
at $b echoat $b .             [1;37m{銀蛇武士堡}[0m
if class($b) == 11
or class($b) == 12
or class($b) == 13
        at $b echoat $b [1;30m(秘)[37m戰士進階任務:[0m
if is_qmark($b) & warrior_quest 100001
or is_qmark($b) & warrior_quest 100002
or is_qmark($b) & warrior_quest 100004
        at $b echoat $b [1;35m--戰士系最終試煉             完成[0m
else
        at $b echoat $b [1;32m--戰士系最終試煉             未完[0m
endif
endif
if is_qmark($b) & warrior_quest 100008
        at $b echoat $b [1;35m抉擇                         完成[0m
else
        at $b echoat $b [1;32m抉擇                         未完[0m
endif
at $b echoat $b .             [1;37m{西方黑森林}[0m
if is_qmark($b) == western_hermit 300005
        at $b echoat $b [1;35m森林隱者的考驗               完成[0m
else
        at $b echoat $b [1;32m森林隱者的考驗               未完[0m
endif
at $b echoat $b .             [1;37m{伊安諾遺跡}[0m
if is_qmark($b) >= ianua_supplies 100004
or is_qmark($b) & ianua_quest 100001
        at $b echoat $b [1;35m物資運送 I                   完成[0m
else
        at $b echoat $b [1;32m物資運送 I                   未完[0m
endif
if is_qmark($b) >= ianua_supplies 100010
or is_qmark($b) & ianua_quest 100002
        at $b echoat $b [1;35m禁斷試驗                     完成[0m
else
        at $b echoat $b [1;32m禁斷試驗                     未完[0m
endif
if is_qmark($b) == ianua_teleporter
        at $b echoat $b [1;35m傳送者之石                   完成[0m
else
        at $b echoat $b [1;32m傳送者之石                   未完[0m
endif
at $b echoat $b .             [1;37m{綠樹王國}[0m
if is_qmark($b) == greenwoods_war_over
        at $b echoat $b [1;35m千里共嬋娟                   完成[0m
else
        at $b echoat $b [1;32m千里共嬋娟                   未完[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100001
        at $b echoat $b [1;35m綠樹英雄第一段               完成[0m
else
        at $b echoat $b [1;32m綠樹英雄第一段               未完[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100002
        at $b echoat $b [1;35m綠樹英雄第二段               完成[0m
else
        at $b echoat $b [1;32m綠樹英雄第二段               未完[0m
endif
if is_qmark($b) >= greenwoods_gharba_finished 100003
        at $b echoat $b [1;35m綠樹英雄第三段               完成[0m
else
        at $b echoat $b [1;32m綠樹英雄第三段               未完[0m
endif
if is_qmark($b) == greenwoods_santa_brother
        at $b echoat $b [1;35m聖塔麻尼卡兄弟會             完成[0m
else
        at $b echoat $b [1;32m聖塔麻尼卡兄弟會             未完[0m
endif
if is_qmark($b) == greenwoods_frowned_quest 100005
        at $b echoat $b [1;35m青澀的愛情                   完成[0m
else
        at $b echoat $b [1;32m青澀的愛情                   未完[0m
endif
at $b echoat $b .             [1;37m{蜘蛛網區}[0m
if is_qmark($b) == arachnos_quest 100007
        at $b echoat $b [1;35m幫助凱倫                     完成[0m
else
        at $b echoat $b [1;32m幫助凱倫                     未完[0m
endif
at $b echoat $b .             [1;37m{屯墾區}[0m
if is_qmark($b) & shire_quest 100002
        at $b echoat $b [1;35m媽媽的金雞蛋(找雞)           完成[0m
else
        at $b echoat $b [1;32m媽媽的金雞蛋(找雞)           未完[0m
endif
if is_qmark($b) & shire_quest 100004
        at $b echoat $b [1;35m媽媽的金雞蛋(下蛋)           完成[0m
else
        at $b echoat $b [1;32m媽媽的金雞蛋(下蛋)           未完[0m
endif
if is_qmark($b) & shire_quest 100001
        at $b echoat $b [1;35m狂龍護符                     完成[0m
else
        at $b echoat $b [1;32m狂龍護符                     未完[0m
endif
def_rembch null-tar
~
>room_defun q04~
at $b echoat $b .             [1;36m[史卡拉貝西邊區域 II][0m
at $b echoat $b .             [1;37m{魔法高塔}[0m
if is_qmark($b) & hitower_questmark 100001
        at $b echoat $b [1;35m水晶玻璃杯                   完成[0m
else
        at $b echoat $b [1;32m水晶玻璃杯                   未完[0m
endif
if is_qmark($b) & hitower_questmark 100002
        at $b echoat $b [1;35m禁忌絃歌                     完成[0m
else
        at $b echoat $b [1;32m禁忌絃歌                     未完[0m
endif
if race($b) == Drow
if is_qmark($b) & hitower_questmark 100004
        at $b echoat $b [1;35m六芒星的赤紅之翼             完成[0m
else
        at $b echoat $b [1;32m六芒星的赤紅之翼             未完[0m
endif
endif
if is_qmark($b) & hitower_questmark 100008
        at $b echoat $b [1;35m舞音與紫電                   完成[0m
else
        at $b echoat $b [1;32m舞音與紫電                   未完[0m
endif
at $b echoat $b .             [1;37m{幽冥深淵}[0m
if is_qmark($b) & murky_quest 100001
        at $b echoat $b [1;35m思鄉的遊子                   完成[0m
else
        at $b echoat $b [1;32m思鄉的遊子                   未完[0m
endif
if is_qmark($b) & murky_quest 100002
        at $b echoat $b [1;35m受傷的小男孩                 完成[0m
else
        at $b echoat $b [1;32m受傷的小男孩                 未完[0m
endif
at $b echoat $b .             [1;37m{禁斷之塔}[0m
if is_qmark($b) == taboo_darkside 100001
        at $b echoat $b [1;35m(秘)戰女神-兀蒂亞之敵        完成[0m
else
if race($b) != Vampire
if is_qmark($b) == taboo_quest 100011
        at $b echoat $b [1;35m還給公主真身                 完成[0m
else
        at $b echoat $b [1;32m還給公主真身                 未完[0m
endif
endif
endif
if is_qmark($b) == taboo_scroll 100002
        at $b echoat $b [1;35m帶克羅回家                   完成[0m
else
        at $b echoat $b [1;32m帶克羅回家                   未完[0m
endif
if is_qmark($b) == taboo_fapei_death
        at $b echoat $b [1;35m炎紅塔傳說                   完成[0m
else
        at $b echoat $b [1;32m炎紅塔傳說                   未完[0m
endif
at $b echoat $b .             [1;37m{海域}[0m
if is_qmark($b) == ocean_captain_quest 300003
or is_qmark($b) & mistocean_quest 100001
        at $b echoat $b [1;35m時空聖戰三部曲(一)           完成[0m
else
        at $b echoat $b [1;32m時空聖戰三部曲(一)           未完[0m
endif
if is_qmark($b) == ocean_cleo_help 100007
or is_qmark($b) == ocean_cleo_help 100009
or is_qmark($b) & mistocean_quest 100002
or is_qmark($b) & mistocean_quest 100004
        at $b echoat $b [1;35m克里歐的委託                 完成[0m
else
        at $b echoat $b [1;32m克里歐的委託                 未完[0m
endif
if is_qmark($b) == ocean_alc_stone 300003
or is_qmark($b) & mistocean_quest 100008
        at $b echoat $b [1;35m賢者之石                     完成[0m
else
        at $b echoat $b [1;32m賢者之石                     未完[0m
endif
at $b echoat $b .             [1;37m{海洋}[0m
if is_qmark($b) == ocean_potion_over 100001
or is_qmark($b) & ocean_quest 300001
        at $b echoat $b [1;35m製造藥水                     完成[0m
else
        at $b echoat $b [1;32m製造藥水                     未完[0m
endif
if is_qmark($b) == ocean_quest 200008
or is_qmark($b) & ocean_quest 300002
        at $b echoat $b [1;35m消滅海怪                     完成[0m
else
        at $b echoat $b [1;32m消滅海怪                     未完[0m
endif
at $b echoat $b .             [1;37m{少林寺}[0m
if is_qmark($b) == shaolin_quest 200003
        at $b echoat $b [1;35m逃獄惡僧                     完成[0m
else
        at $b echoat $b [1;32m逃獄惡僧                     未完[0m
endif
def_rembch null-tar
~
>room_defun q05~
at $b echoat $b .             [1;36m[史卡拉貝東邊區域及大沙漠區 I][0m
at $b echoat $b .             [1;37m{神聖樹林}[0m
if is_qmark($b) & grove_quest 100001
        at $b echoat $b [1;35m老頭來托夢                   完成[0m
else
        at $b echoat $b [1;32m老頭來托夢                   未完[0m
endif
at $b echoat $b .             [1;37m{精靈之森}[0m
if is_qmark($b) == elftown_quest_over 100001
        at $b echoat $b [1;35m精靈決戰地靈                 完成[0m
else
        at $b echoat $b [1;32m精靈決戰地靈                 未完[0m
endif
if is_qmark($b) == war_reward 300100
        at $b echoat $b [1;35m精靈英雄                     完成[0m
else
        at $b echoat $b [1;32m精靈英雄                     未完[0m
endif
at $b echoat $b .             [1;37m{地靈洞窟}[0m
if is_qmark($b) & kobold_cave_hero 300512
        at $b echoat $b [1;35m地靈英雄                     完成[0m
else
        at $b echoat $b [1;32m地靈英雄                     未完[0m
endif
at $b echoat $b .             [1;37m{賽伐斯拖普}[0m
if is_qmark($b) & hobbit_all_quests 100016
        at $b echoat $b [1;35m盜賊大師                     完成[0m
else
        at $b echoat $b [1;32m盜賊大師                     未完[0m
endif
if is_qmark($b) & hobbit_all_quests 100008
        at $b echoat $b [1;35m情理法的抉擇                 完成[0m
else
        at $b echoat $b [1;32m情理法的抉擇                 未完[0m
endif
if is_qmark($b) & hobbit_all_quests 100001
        at $b echoat $b [1;35m大陸旅行好手 I               完成[0m
else
        at $b echoat $b [1;32m大陸旅行好手 I               未完[0m
endif
if is_qmark($b) & hobbit_all_quests 100002
        at $b echoat $b [1;35m大陸旅行好手 II              完成[0m
else
        at $b echoat $b [1;32m大陸旅行好手 II              未完[0m
endif
if is_qmark($b) & hobbit_all_quests 100004
        at $b echoat $b [1;35m魔戒                         完成[0m
else
        at $b echoat $b [1;32m魔戒                         未完[0m
endif
at $b echoat $b .             [1;37m{泰洛斯}[0m
if is_qmark($b) == thalos_practice_quest 100001
        at $b echoat $b [1;35m大魔導的苦痛                 完成[0m
else
        at $b echoat $b [1;32m大魔導的苦痛                 未完[0m
endif
if is_qmark($b) == thalos_mayor_quest 100005
        at $b echoat $b [1;35m守衛隊長的懺悔               完成[0m
else
        at $b echoat $b [1;32m守衛隊長的懺悔               未完[0m
endif
if is_qmark($b) & thalos_darkforce_quest 132768
or is_qmark($b) & thalos_darkforce_quest 165536
        at $b echoat $b [1;35m消滅黑暗勢力                 完成[0m
else
        at $b echoat $b [1;32m消滅黑暗勢力                 未完[0m
endif
at $b echoat $b .             [1;37m{尤根王國}[0m
if is_qmark($b) == juargan_quest 100012
        at $b echoat $b [1;35m取回權杖                     完成[0m
else
        at $b echoat $b [1;32m取回權杖                     未完[0m
endif
at $b echoat $b .             [1;37m{人馬村}[0m
if is_qmark($b) == wyvern_tower 100011
        at $b echoat $b [1;35m清除飛龍塔                   完成[0m
else
        at $b echoat $b [1;32m清除飛龍塔                   未完[0m
endif
if is_qmark($b) == wyvern_tower 200016
        at $b echoat $b [1;35m龍蜥出沒注意                 完成[0m
else
        at $b echoat $b [1;32m龍蜥出沒注意                 未完[0m
endif
if is_qmark($b) == wyvern_tower 300031
        at $b echoat $b [1;35m靈魂鏈結                     完成[0m
else
        at $b echoat $b [1;32m靈魂鏈結                     未完[0m
endif
at $b echoat $b .             [1;37m{矮人陵墓}[0m
if is_qmark($b) == cata_quest2_explorer 100005
        at $b echoat $b [1;35m研究員的研究                 完成[0m
else
        at $b echoat $b [1;32m研究員的研究                 未完[0m
endif
if is_qmark($b) == cata_quest_templar 100006
        at $b echoat $b [1;35m聖殿的求救                   完成[0m
else
        at $b echoat $b [1;32m聖殿的求救                   未完[0m
endif
at $b echoat $b .             [1;37m{人偶鎮}[0m
if is_qmark($b) == muppet_grover_save 100006
        at $b echoat $b [1;35m拯救人偶鎮                   完成[0m
else
        at $b echoat $b [1;32m拯救人偶鎮                   未完[0m
endif
at $b echoat $b .             [1;37m{時雨地墓}[0m
if is_qmark($b) & cemetery_mark2 200002
        at $b echoat $b [1;35m債務糾紛                     完成[0m
else
        at $b echoat $b [1;32m債務糾紛                     未完[0m
endif
if is_qmark($b) & cemetery_mark2 200004
        at $b echoat $b [1;35m死者怨念                     完成[0m
else
        at $b echoat $b [1;32m死者怨念                     未完[0m
endif
def_rembch null-tar
~
>room_defun q06~
at $b echoat $b .             [1;36m[史卡拉貝東邊區域及大沙漠區 II][0m
at $b echoat $b .             [1;37m{永恆之塔}[0m
if class($b) == 0
or class($b) == 1
or class($b) == 5
or class($b) == 6
or class($b) == 7
or class($b) == 8
or class($b) == 15
or class($b) == 16
or class($b) == 14
        at $b echoat $b [1;30m(秘)[37m法師基本任務:[0m
if is_qmark($b) & eternity_quest 100001
        at $b echoat $b [1;35m--魔法初級檢定               完成[0m
else
        at $b echoat $b [1;32m--魔法初級檢定               未完[0m
endif
endif
if is_qmark($b) & eternity_quest 100002
        at $b echoat $b [1;35m父親的金錶                   完成[0m
else
        at $b echoat $b [1;32m父親的金錶                   未完[0m
endif
if is_qmark($b) & eternity_quest 100004
        at $b echoat $b [1;35m困難的作業                   完成[0m
else
        at $b echoat $b [1;32m困難的作業                   未完[0m
endif
if is_qmark($b) & eternity_quest 100008
        at $b echoat $b [1;35m遊子吟                       完成[0m
else
        at $b echoat $b [1;32m遊子吟                       未完[0m
endif
if is_qmark($b) & eternity_quest 100016
        at $b echoat $b [1;35m狂人們的爭執                 完成[0m
else
        at $b echoat $b [1;32m狂人們的爭執                 未完[0m
endif
if is_qmark($b) & eternity_quest 100032
        at $b echoat $b [1;35m五芒召喚陣                   完成[0m
else
        at $b echoat $b [1;32m五芒召喚陣                   未完[0m
endif
if class($b) == 0
or class($b) == 5
or class($b) == 6
or class($b) == 15
        at $b echoat $b [1;30m(秘)[37m組織任務:[0m
if is_qmark($b) >= wfs_clan_member 100001
        at $b echoat $b [1;35m--風火海現世                 完成[0m
else
        at $b echoat $b [1;32m--風火海現世                 未完[0m
endif
endif
if is_qmark($b) & eternity_quest 100064
        at $b echoat $b [1;35m製造魔力法杖                 完成[0m
else
        at $b echoat $b [1;32m製造魔力法杖                 未完[0m
endif
if is_qmark($b) & eternity_quest 100128
        at $b echoat $b [1;35m危機                         完成[0m
else
        at $b echoat $b [1;32m危機                         未完[0m
endif
if is_qmark($b) & eternity_quest 100256
        at $b echoat $b [1;35m小精靈的願望                 完成[0m
else
        at $b echoat $b [1;32m小精靈的願望                 未完[0m
endif
at $b echoat $b .             [1;37m{曼托爾堡壘}[0m
if is_qmark($b) != mahntor_ogre_quest2 100017
if is_qmark($b) == mahntor_ogre_quest1 200004
or is_qmark($b) == mahntor_ogre_quest1 100004
        at $b echoat $b [1;35m酋長的心事                   完成[0m
else
        at $b echoat $b [1;32m酋長的心事                   未完[0m
endif
endif
if is_qmark($b) != mahntor_ogre_quest2 100017
if is_qmark($b) == mahntor_shaman_nut 100004
        at $b echoat $b [1;35m老年痴呆的巫醫               完成[0m
else
        at $b echoat $b [1;32m老年痴呆的巫醫               未完[0m
endif
endif
if is_qmark($b) != mahntor_ogre_quest1 200004
if is_qmark($b) != mahntor_ogre_quest1 100004
if is_qmark($b) == mahntor_ogre_quest2 100017
        at $b echoat $b [1;35m堡壘的危機                   完成[0m
else
        at $b echoat $b [1;32m堡壘的危機                   未完[0m
endif
endif
endif
at $b echoat $b .             [1;37m{蠻荒森林}[0m
if is_qmark($b) == forest_quest
        at $b echoat $b [1;35m學習土人語言                 完成[0m
else
        at $b echoat $b [1;32m學習土人語言                 未完[0m
endif
if is_qmark($b) == mist_clean_quest 100020
        at $b echoat $b [1;35m生病的土人                   完成[0m
else
        at $b echoat $b [1;32m生病的土人                   未完[0m
endif
if is_qmark($b) & forest_quest 300128
        at $b echoat $b [1;35m酋長的託付                   完成[0m
else
        at $b echoat $b [1;32m酋長的託付                   未完[0m
endif
if is_qmark($b) >= forest_quest 100004
        at $b echoat $b [1;35m火焰神珠                     完成[0m
else
        at $b echoat $b [1;32m火焰神珠                     未完[0m
endif
if is_qmark($b) == forest_quest 100007
or is_qmark($b) & forest_quest 101024
        at $b echoat $b [1;35m火龍賽巴洛司                 完成[0m
else
        at $b echoat $b [1;32m火龍賽巴洛司                 未完[0m
endif
if class($b) == 14
or class($b) == 18
or class($b) == 19
        at $b echoat $b [1;30m(秘)[37m蠻俠試煉任務:[0m
if is_qmark($b) == forest_barbarian_exam 100002
        at $b echoat $b [1;35m--野蠻人試煉                 完成[0m
else
        at $b echoat $b [1;32m--野蠻人試煉                 未完[0m
endif
endif
at $b echoat $b .             [1;37m{麥格城}[0m
if is_qmark($b) == mega1_arena_owner 300001
        at $b echoat $b [1;35m競技場之謎                   完成[0m
else
        at $b echoat $b [1;32m競技場之謎                   未完[0m
endif
if is_qmark($b) == mega1_alibaba_hunting 100001
        at $b echoat $b [1;35m獵殺阿里巴巴                 完成[0m
else
        at $b echoat $b [1;32m獵殺阿里巴巴                 未完[0m
endif
if is_qmark($b) == mega1_mayor_vsjudge 100004
        at $b echoat $b [1;35m貝多的手杖                   完成[0m
else
        at $b echoat $b [1;32m貝多的手杖                   未完[0m
endif
at $b echoat $b .             [1;37m{艾斯貝爾傭兵團營地}[0m
if is_qmark($b) & sandrock_questmark 100001
        at $b echoat $b [1;35m異界語言                     完成[0m
else
        at $b echoat $b [1;32m異界語言                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100002
        at $b echoat $b [1;35m金屬收購                     完成[0m
else
        at $b echoat $b [1;32m金屬收購                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100004
        at $b echoat $b [1;35m兵器鑄造                     完成[0m
else
        at $b echoat $b [1;32m兵器鑄造                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100008
        at $b echoat $b [1;35m魔法摺紙                     完成[0m
else
        at $b echoat $b [1;32m魔法摺紙                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100016
        at $b echoat $b [1;35m飼料調配                     完成[0m
else
        at $b echoat $b [1;32m飼料調配                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100032
        at $b echoat $b [1;35m枯井調查                     完成[0m
else
        at $b echoat $b [1;32m枯井調查                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100064
        at $b echoat $b [1;35m安撫請求                     完成[0m
else
        at $b echoat $b [1;32m安撫請求                     未完[0m
endif
if is_qmark($b) & sandrock_questmark 100128
        at $b echoat $b [1;35m尋找工具                     完成[0m
else
        at $b echoat $b [1;32m尋找工具                     未完[0m
endif
def_rembch null-tar
~
>room_defun q07~
at $b echoat $b .             [1;36m[平行世界以及外圍地區 I][0m
at $b echoat $b .             [1;37m{創世紀}[0m
if is_qmark($b) >= ultima_destroy_evil 100008
        at $b echoat $b [1;35m八大美德的考驗               完成[0m
else
        at $b echoat $b [1;32m八大美德的考驗               未完[0m
endif
if is_qmark($b) == ultima_destroy_evil 100011
        at $b echoat $b [1;35m創世紀聖戰                   完成[0m
else
        at $b echoat $b [1;32m創世紀聖戰                   未完[0m
endif
at $b echoat $b .             [1;37m{奧林帕斯山}[0m
if is_qmark($b) == olympus_weapon_quest 200009
or is_qmark($b) == olympus_weapon_quest 100009
        at $b echoat $b [1;35m天界之爭                     完成[0m
else
        at $b echoat $b [1;32m天界之爭                     未完[0m
endif
if is_qmark($b) == olympus_jail_quest 100021
        at $b echoat $b [1;35m釋放囚犯                     完成[0m
else
        at $b echoat $b [1;32m釋放囚犯                     未完[0m
endif
at $b echoat $b .             [1;37m{聖域}[0m
if is_qmark($b) == saintarea_quest_main 100040
        at $b echoat $b [1;35m黃金十二宮考驗               完成[0m
else
        at $b echoat $b [1;32m黃金十二宮考驗               未完[0m
endif
if is_qmark($b) == saintarea_quest_geminimask 100002
        at $b echoat $b [1;35m打造聖衣                     完成[0m
else
        at $b echoat $b [1;32m打造聖衣                     未完[0m
endif
if is_qmark($b) == saintarea_perseus 100019
        at $b echoat $b [1;35m打造神盾                     完成[0m
else
        at $b echoat $b [1;32m打造神盾                     未完[0m
endif
at $b echoat $b .             [1;37m{星界}[0m
if is_qmark($b) & astral_quest 100001
        at $b echoat $b [1;35m拯救星界                     完成[0m
else
        at $b echoat $b [1;32m拯救星界                     未完[0m
endif
if is_qmark($b) & astral_quest 100002
        at $b echoat $b [1;35m拯救紅龍                     完成[0m
else
        at $b echoat $b [1;32m拯救紅龍                     未完[0m
endif
if is_qmark($b) & astral_quest 100004
        at $b echoat $b [1;35m打造護甲                     完成[0m
else
        at $b echoat $b [1;32m打造護甲                     未完[0m
endif
at $b echoat $b .             [1;37m{星光之城}[0m
if is_qmark($b) & thoran_quest 100001
        at $b echoat $b [1;35m援救奈亞                     完成[0m
else
        at $b echoat $b [1;32m援救奈亞                     未完[0m
endif
if is_qmark($b) & thoran_quest 100002
        at $b echoat $b [1;35m主堡通行證                   完成[0m
else
        at $b echoat $b [1;32m主堡通行證                   未完[0m
endif
if is_qmark($b) & thoran_quest 100004
        at $b echoat $b [1;35m酒店武士的義眼               完成[0m
else
        at $b echoat $b [1;32m酒店武士的義眼               未完[0m
endif
if is_qmark($b) & thoran_quest 100008
        at $b echoat $b [1;35m憂傷守衛的願望               完成[0m
else
        at $b echoat $b [1;32m憂傷守衛的願望               未完[0m
endif
if is_qmark($b) & thoran_quest 100016
        at $b echoat $b [1;35m被下詛咒的堡主               完成[0m
else
        at $b echoat $b [1;32m被下詛咒的堡主               未完[0m
endif
def_rembch null-tar
~
>room_defun q08~
at $b echoat $b .             [1;36m[平行世界以及外圍地區 II][0m
at $b echoat $b .             [1;37m{南極大陸}[0m
if is_qmark($b) & southend_quest 100002
        at $b echoat $b [1;35m布布的貪吃                   完成[0m
else
        at $b echoat $b [1;32m布布的貪吃                   未完[0m
endif
if is_qmark($b) & southend_quest 100004
        at $b echoat $b [1;35m肯特的煩惱                   完成[0m
else
        at $b echoat $b [1;32m肯特的煩惱                   未完[0m
endif
if is_qmark($b) & southend_quest 100008
        at $b echoat $b [1;35m村莊的危機                   完成[0m
else
        at $b echoat $b [1;32m村莊的危機                   未完[0m
endif
if is_qmark($b) & southend_quest 100016
        at $b echoat $b [1;35m瑪莉的迷糊                   完成[0m
else
        at $b echoat $b [1;32m瑪莉的迷糊                   未完[0m
endif
if is_qmark($b) & southend_quest 100032
        at $b echoat $b [1;35m神器的重現                   完成[0m
else
        at $b echoat $b [1;32m神器的重現                   未完[0m
endif
if is_qmark($b) & southend_quest 100064
        at $b echoat $b [1;35m惱人小狼犬                   完成[0m
else
        at $b echoat $b [1;32m惱人小狼犬                   未完[0m
endif
def_rembch null-tar
~
>room_defun q09~
at $b echoat $b .             [1;36m[迷霧之島主區][0m
at $b echoat $b .             [1;37m{光明之鎮}[0m
if is_qmark($b) & mist_quest 100001
        at $b echoat $b [1;35m製造藥水                     完成[0m
else
        at $b echoat $b [1;32m製造藥水                     未完[0m
endif
if is_qmark($b) & mist_quest 100002
        at $b echoat $b [1;35m死亡谷之旅                   完成[0m
else
        at $b echoat $b [1;32m死亡谷之旅                   未完[0m
endif
if is_qmark($b) & mist_quest 100004
        at $b echoat $b [1;35m光明之鎮垃圾事件             完成[0m
else
        at $b echoat $b [1;32m光明之鎮垃圾事件             未完[0m
endif
if is_qmark($b) == mist_protector 100001
        at $b echoat $b [1;35m消滅恐龍                     完成[0m
else
        at $b echoat $b [1;32m消滅恐龍                     未完[0m
endif
if is_qmark($b) & mist_council_over 100002
        at $b echoat $b [1;35m市議會任務--簡易送貨 I       完成[0m
else
        at $b echoat $b [1;32m市議會任務--簡易送貨 I       未完[0m
endif
if is_qmark($b) & mist_council_over 100004
        at $b echoat $b [1;35m市議會任務--簡易送貨 II      完成[0m
else
        at $b echoat $b [1;32m市議會任務--簡易送貨 II      未完[0m
endif
if is_qmark($b) & mist_council_over 100008
        at $b echoat $b [1;35m市議會任務--暗殺 I           完成[0m
else
        at $b echoat $b [1;32m市議會任務--暗殺 I           未完[0m
endif
at $b echoat $b .             [1;37m{水晶湖}[0m
if is_qmark($b) >= crystal_reward 100001
        at $b echoat $b [1;35m拯救銀龍                     完成[0m
else
        at $b echoat $b [1;32m拯救銀龍                     未完[0m
endif
at $b echoat $b .             [1;37m{楓紅林}[0m
if is_qmark($b) == maple_ack 100004
        at $b echoat $b [1;35m自然的定律                   完成[0m
else
        at $b echoat $b [1;32m自然的定律                   未完[0m
endif
if is_qmark($b) == maple_camus_attack 100003
        at $b echoat $b [1;35m幽冥深淵的使徒               完成[0m
else
        at $b echoat $b [1;32m幽冥深淵的使徒               未完[0m
endif
if class($b) == 12
        at $b echoat $b [1;30m(秘)[37m劍士進階任務:[0m
if is_qmark($b) == maple_swd_trail 100006
        at $b echoat $b [1;35m--劍士進階試煉               完成[0m
else
        at $b echoat $b [1;32m--劍士進階試煉               未完[0m
endif
endif
if class($b) == 2
or class($b) == 9
or class($b) == 10
or class($b) == 17
at $b echoat $b .             [1;37m{盜賊要塞}[0m
if is_qmark($b) >= bandit_thief_exam 100005
        at $b echoat $b [1;35m盜賊守則                     完成[0m
else
        at $b echoat $b [1;32m盜賊守則                     未完[0m
endif
endif
if class($b) == 10
        at $b echoat $b [1;30m(秘)[37m忍者進階任務:[0m
if is_qmark($b) == bandit_thief_exam 100007
        at $b echoat $b [1;35m--忍者試煉                   完成[0m
else
        at $b echoat $b [1;32m--忍者試煉                   未完[0m
endif
endif
def_rembch null-tar
~
>room_defun q10~
at $b echoat $b .             [1;36m[迷霧之島奈圖王國][0m
at $b echoat $b .             [1;37m{奈圖城堡}[0m
if is_qmark($b) >= naytil_quest 100008
        at $b echoat $b [1;35m圖書館中的惡魔               完成[0m
else
        at $b echoat $b [1;32m圖書館中的惡魔               未完[0m
endif
if is_qmark($b) != fight_naytil 100008
if is_qmark($b) == fight_highland 100005
        at $b echoat $b [1;35m決戰高地                     完成[0m
else
        at $b echoat $b [1;32m決戰高地                     未完[0m
endif
endif
if is_qmark($b) == naytil_princess 100001
        at $b echoat $b [1;35m尋找小貓                     完成[0m
else
        at $b echoat $b [1;32m尋找小貓                     未完[0m
endif
if is_qmark($b) == naytil_rebels 200001
or is_qmark($b) == naytil_rebels 200002
        at $b echoat $b [1;35m奈圖/反叛軍大戰篇            完成[0m
else
        at $b echoat $b [1;32m奈圖/反叛軍大戰篇            未完[0m
endif
at $b echoat $b .             [1;37m{高地}[0m
if is_qmark($b) != fight_highland 100005
if is_qmark($b) == fight_naytil 100008
        at $b echoat $b [1;35m決戰奈圖                     完成[0m
else
        at $b echoat $b [1;32m決戰奈圖                     未完[0m
endif
endif
if is_qmark($b) == for_rose 100005
        at $b echoat $b [1;35m玫瑰之書                     完成[0m
else
        at $b echoat $b [1;32m玫瑰之書                     未完[0m
endif
at $b echoat $b .             [1;37m{索雷斯城}[0m
if is_qmark($b) & mist_quest 100008
        at $b echoat $b [1;35m冒險者的願望                 完成[0m
else
        at $b echoat $b [1;32m冒險者的願望                 未完[0m
endif
at $b echoat $b .             [1;37m{鬼鎮}[0m
if is_qmark($b) & gtown_quest_list 100001
        at $b echoat $b [1;35m死亡谷入侵                   完成[0m
else
        at $b echoat $b [1;32m死亡谷入侵                   未完[0m
endif
if race($b) == vampire
if is_qmark($b) == gtown_vampire_finish
        at $b echoat $b [1;35m吸血鬼之戀                   完成[0m
else
        at $b echoat $b [1;32m吸血鬼之戀                   未完[0m
endif
endif
if is_qmark($b) & gtown_quest_list 100002
        at $b echoat $b [1;35m小販三兄弟之友               完成[0m
else
        at $b echoat $b [1;32m小販三兄弟之友               未完[0m
endif
if is_qmark($b) & gtown_quest_list 100004
        at $b echoat $b [1;35m夢幻之花                     完成[0m
else
        at $b echoat $b [1;32m夢幻之花                     未完[0m
endif
if is_qmark($b) & gtown_quest_list 100008
        at $b echoat $b [1;35m小說家的誕生                 完成[0m
else
        at $b echoat $b [1;32m小說家的誕生                 未完[0m
endif
def_rembch null-tar
~
>room_defun q11~
at $b echoat $b .             [1;36m[迷霧之島之其他區域 I][0m
at $b echoat $b .             [1;37m{遠古森林}[0m
if is_qmark($b) & prehist_quest 100001
        at $b echoat $b [1;35m拯救野獸                     完成[0m
else
        at $b echoat $b [1;32m拯救野獸                     未完[0m
endif
if is_qmark($b) & prehist_quest 100002
        at $b echoat $b [1;35m取悅冰長老                   完成[0m
else
        at $b echoat $b [1;32m取悅冰長老                   未完[0m
endif
if is_qmark($b) & prehist_quest 100004
        at $b echoat $b [1;35m為小痞取得配刀               完成[0m
else
        at $b echoat $b [1;32m為小痞取得配刀               未完[0m
endif
if is_qmark($b) & prehist_quest 100008
        at $b echoat $b [1;35m幫路克修理房子               完成[0m
else
        at $b echoat $b [1;32m幫路克修理房子               未完[0m
endif
if is_qmark($b) & prehist_quest 100016
        at $b echoat $b [1;35m小女孩的寶藏                 完成[0m
else
        at $b echoat $b [1;32m小女孩的寶藏                 未完[0m
endif
if is_qmark($b) & prehist_quest 100032
        at $b echoat $b [1;35m幫助遠古森林中冒險者脫離     完成[0m
else
        at $b echoat $b [1;32m幫助遠古森林中冒險者脫離     未完[0m
endif
if is_qmark($b) & prehist_quest 100064
        at $b echoat $b [1;35m取得獨眼魚與大雕蛋           完成[0m
else
        at $b echoat $b [1;32m取得獨眼魚與大雕蛋           未完[0m
endif
if is_qmark($b) & prehist_quest 100128
        at $b echoat $b [1;35m清除冰庫內生物               完成[0m
else
        at $b echoat $b [1;32m清除冰庫內生物               未完[0m
endif
if is_qmark($b) & prehist_quest 100512
        at $b echoat $b [1;35m霧老師的煩惱                 完成[0m
else
        at $b echoat $b [1;32m霧老師的煩惱                 未完[0m
endif
at $b echoat $b .             [1;37m{封魔洞窟}[0m
if class($b) == 7
or class($b) == 8
if is_qmark($b) & sealevil_quest 100001
        at $b echoat $b [1;35m守護者晶石                   完成[0m
else
        at $b echoat $b [1;32m守護者晶石                   未完[0m
endif
endif
if is_qmark($b) & sealevil_quest 100002
or is_qmark($b) > sealevil_Aleru_quest 100018
        at $b echoat $b [1;35m家族的使命第一段             完成[0m
else
        at $b echoat $b [1;32m家族的使命第一段             未完[0m
endif
if is_qmark($b) & sealevil_quest 100002
        at $b echoat $b [1;35m家族的使命第二段             完成[0m
else
        at $b echoat $b [1;32m家族的使命第二段             未完[0m
endif
if is_qmark($b) & sealevil_quest 100004
        at $b echoat $b [1;35m姊妹恩仇錄                   完成[0m
else
        at $b echoat $b [1;32m姊妹恩仇錄                   未完[0m
endif
if is_qmark($b) & sealevil_quest 100008
        at $b echoat $b [1;35m精靈之鑽                     完成[0m
else
        at $b echoat $b [1;32m精靈之鑽                     未完[0m
endif
if is_qmark($b) & sealevil_quest 100016
        at $b echoat $b [1;35m雙刃合一                     完成[0m
else
        at $b echoat $b [1;32m雙刃合一                     未完[0m
endif
at $b echoat $b .             [1;37m{雪華城堡}[0m
if is_qmark($b) & snow_questcheck 100001
or is_qmark($b) == snow_Porak01_quest 100011
        at $b echoat $b [1;35m秘密間諜                     完成[0m
else
        at $b echoat $b [1;32m秘密間諜                     未完[0m
endif
if is_qmark($b) & snow_questcheck 100002
or is_qmark($b) == snow_Porak02_quest 100010
        at $b echoat $b [1;35m殺手                         完成[0m
else
        at $b echoat $b [1;32m殺手                         未完[0m
endif
if is_qmark($b) & snow_questcheck 100004
or is_qmark($b) == snow_Porak03_quest 100015
        at $b echoat $b [1;35m第四神器                     完成[0m
else
        at $b echoat $b [1;32m第四神器                     未完[0m
endif
if is_qmark($b) & snow_questcheck 100008
or is_qmark($b) == sealevil_Tylas_quest 100015
        at $b echoat $b [1;35m精靈之鑽II                   完成[0m
else
        at $b echoat $b [1;32m精靈之鑽II                   未完[0m
endif
if is_qmark($b) & snow_questcheck 100016
or is_qmark($b) >= sealevil_Aleru_quest 100089
        at $b echoat $b [1;35m家族的使命第三段             完成[0m
else
        at $b echoat $b [1;32m家族的使命第三段             未完[0m
endif
def_rembch null-tar
~
>room_defun q12~
at $b echoat $b .             [1;36m[迷霧之島之其他區域 II][0m
at $b echoat $b .             [1;37m{愛潔梅菲島}[0m
if is_qmark($b) & Ageemephy_questmark 200001
        at $b echoat $b [1;35m未降臨的神諭‧謊言           完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100001
                at $b echoat $b [1;35m未降臨的神諭                 完成[0m
        else
                at $b echoat $b [1;32m未降臨的神諭                 未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200002
        at $b echoat $b [1;35m失職兵甲官‧記憶             完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100002
                at $b echoat $b [1;35m失職兵甲官                   完成[0m
        else
                at $b echoat $b [1;32m失職兵甲官                   未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200004
        at $b echoat $b [1;35m惡魔鬥士‧背叛               完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100004
                at $b echoat $b [1;35m惡魔鬥士                     完成[0m
        else
                at $b echoat $b [1;32m惡魔鬥士                     未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200008
        at $b echoat $b [1;35m黑武者之劍‧救贖             完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100008
                at $b echoat $b [1;35m黑武者之劍                   完成[0m
        else
                at $b echoat $b [1;32m黑武者之劍                   未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200016
        at $b echoat $b [1;35m聖邪皆具的少女‧疾走         完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100016
                at $b echoat $b [1;35m聖邪皆具的少女               完成[0m
        else
                at $b echoat $b [1;32m聖邪皆具的少女               未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200032
        at $b echoat $b [1;35m魔法陣研究‧悲願             完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100032
                at $b echoat $b [1;35m魔法陣研究                   完成[0m
        else
                at $b echoat $b [1;32m魔法陣研究                   未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200064
        at $b echoat $b [1;35m先賢列傳‧榮耀               完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100064
                at $b echoat $b [1;35m先賢列傳                     完成[0m
        else
                at $b echoat $b [1;32m先賢列傳                     未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200128
        at $b echoat $b [1;35m沉睡在靜止的時光中‧夢音     完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100128
                at $b echoat $b [1;35m沉睡在靜止的時光中           完成[0m
        else
                at $b echoat $b [1;32m沉睡在靜止的時光中           未完[0m
        endif
endif
if is_qmark($b) & Ageemephy_questmark 200256
        at $b echoat $b [1;35m追尋前人的腳步‧軌跡         完成[0m
else
        if is_qmark($b) & Ageemephy_questmark 100256
                at $b echoat $b [1;35m追尋前人的腳步               完成[0m
        else
                at $b echoat $b [1;32m追尋前人的腳步               未完[0m
        endif
endif
at $b echoat $b .             [1;37m{愛潔梅菲島周邊}[0m
if is_qmark($b) & TideIsland_questmark 200001
        at $b echoat $b [1;35m被封印之古祭壇               完成[0m
else
        at $b echoat $b [1;32m被封印之古祭壇               未完[0m
endif
if class($b) == 11
or class($b) == 16
if is_qmark($b) & TideIsland_questmark 200002
        at $b echoat $b [1;35m獵熊傳說                     完成[0m
else
        at $b echoat $b [1;32m獵熊傳說                     未完[0m
endif
endif
if is_qmark($b) >= sealevil_Aleru_quest 100104
        at $b echoat $b [1;35m家族的使命第四段             完成[0m
else
        at $b echoat $b [1;32m家族的使命第四段             未完[0m
endif
at $b echoat $b .             [1;37m{幽暗密林}[0m
if is_qmark($b) == mirkwood_quest 100006
        at $b echoat $b [1;35m逃獄                         完成[0m
else
        at $b echoat $b [1;32m逃獄                         未完[0m
endif
at $b echoat $b .             [1;37m{瑪拉勒城}[0m
if is_qmark($b) >= malathar_quest 100003
        at $b echoat $b [1;35m小惡鬼偵察部隊               完成[0m
else
        at $b echoat $b [1;32m小惡鬼偵察部隊               未完[0m
endif
if is_qmark($b) >= malathar_quest 100008
        at $b echoat $b [1;35m魔力捐獻                     完成[0m
else
        at $b echoat $b [1;32m魔力捐獻                     未完[0m
endif
if is_qmark($b) >= malathar_quest 100012
        at $b echoat $b [1;35m間諜清除戰                   完成[0m
else
        at $b echoat $b [1;32m間諜清除戰                   未完[0m
endif
if is_qmark($b) >= malathar_quest 100029
        at $b echoat $b [1;35m議和                         完成[0m
else
        at $b echoat $b [1;32m議和                         未完[0m
endif
if is_qmark($b) >= malathar_quest 100033
        at $b echoat $b [1;35m烽火時刻                     完成[0m
else
        at $b echoat $b [1;32m烽火時刻                     未完[0m
endif
if is_qmark($b) >= malathar_quest 100038
        at $b echoat $b [1;35m神護之力                     完成[0m
else
        at $b echoat $b [1;32m神護之力                     未完[0m
endif
if is_qmark($b) == malathar_quest 100043
        at $b echoat $b [1;35m黑暗的實體                   完成[0m
else
        at $b echoat $b [1;32m黑暗的實體                   未完[0m
endif
def_rembch null-tar
~
>room_defun q13~
at $b echoat $b .             [1;36m[迷霧之島迷斯卓諾區域][0m
at $b echoat $b .             [1;37m{迷斯卓諾谷地}[0m
if is_qmark($b) >= myth_seamstress 100020
or is_qmark($b) & myth_greenleaf_end 100001
        at $b echoat $b [1;35m職人精神                     完成[0m
else
        at $b echoat $b [1;32m職人精神                     未完[0m
endif
if is_qmark($b) & myth_greenleaf_end 100002
        at $b echoat $b [1;35m魔力界限突破研究             完成[0m
else
        at $b echoat $b [1;32m魔力界限突破研究             未完[0m
endif
if is_qmark($b) & myth_greenleaf_end 100004
or is_qmark($b) & myth_greenleaf_end 200004
        at $b echoat $b [1;35m翹班的行政長官               完成[0m
else
        at $b echoat $b [1;32m翹班的行政長官               未完[0m
endif
if is_qmark($b) & myth_family_recipe 300001
        at $b echoat $b [1;35m追悼先人                     完成[0m
else
        at $b echoat $b [1;32m追悼先人                     未完[0m
endif
if is_qmark($b) & myth_family_recipe 300002
        at $b echoat $b [1;35m應受繼承之物                 完成[0m
else
        at $b echoat $b [1;32m應受繼承之物                 未完[0m
endif
if is_qmark($b) & myth_elven_end 100001
or is_qmark($b) & myth_elven_end 200001
        at $b echoat $b [1;35m失落的篇章                   完成[0m
else
        at $b echoat $b [1;32m失落的篇章                   未完[0m
endif
if is_qmark($b) & myth_templar_end 100001
or is_qmark($b) & myth_templar_end 200001
        at $b echoat $b [1;35m教團調查隊                   完成[0m
else
        at $b echoat $b [1;32m教團調查隊                   未完[0m
endif
if is_qmark($b) & myth_templar_end 100002
or is_qmark($b) & myth_templar_end 200002
        at $b echoat $b [1;35m行蹤不明的騎士               完成[0m
else
        at $b echoat $b [1;32m行蹤不明的騎士               未完[0m
endif
if is_qmark($b) & myth_templar_end 100004
or is_qmark($b) & myth_templar_end 200004
        at $b echoat $b [1;35m教團調查隊II                 完成[0m
else
        at $b echoat $b [1;32m教團調查隊II                 未完[0m
endif
if is_qmark($b) == myth_templar_enemy
or is_qmark($b) == templar_absolution
        if is_qmark($b) == templar_absolution 100005
                at $b echoat $b [1;35m教會贖罪券                   完成[0m
        else
                at $b echoat $b [1;32m教會贖罪券                   未完[0m
        endif
endif
at $b echoat $b .             [1;37m{暗黑精靈領地}[0m
if is_qmark($b) == enclave_quest 100007
        at $b echoat $b [1;35m尋找人質                     完成[0m
else
        at $b echoat $b [1;32m尋找人質                     未完[0m
endif
def_rembch null-tar
~
>room_defun boss_list~
at $b echoat $b [1;37m你可以查看的目標物區域:[0m
at $b echoat $b 第一頁 [1;32m史卡拉貝及週邊區域 I           [37m(    boss01     )[0m
at $b echoat $b 第二頁 [1;32m史卡拉貝及週邊區域 II          [37m(    boss02     )[0m
at $b echoat $b 第三頁 空頁
at $b echoat $b 第四頁 [1;32m史卡拉貝西邊區域 I             [37m(    boss04     )[0m
at $b echoat $b 第五頁 [1;32m史卡拉貝西邊區域 II            [37m(    boss05     )[0m
at $b echoat $b 第六頁 空頁
at $b echoat $b 第七頁 [1;32m史卡拉貝東邊區域及大沙漠區 I   [37m(    boss07     )[0m
at $b echoat $b 第八頁 [1;32m史卡拉貝東邊區域及大沙漠區 II  [37m(    boss08     )[0m
at $b echoat $b 第九頁 空頁
at $b echoat $b 第十頁 [1;32m平行世界以及外圍地區 I         [37m(    boss10     )[0m
at $b echoat $b 第11頁 [1;32m平行世界以及外圍地區 II        [37m(    boss11     )[0m
at $b echoat $b 第12頁 空頁
at $b echoat $b 第13頁 [1;36m迷霧之島主區                   [37m(    boss13     )[0m
at $b echoat $b 第14頁 空頁
at $b echoat $b 第15頁 空頁
at $b echoat $b 第16頁 [1;36m迷霧之島奈圖王國               [37m(    boss16     )[0m
at $b echoat $b 第17頁 空頁
at $b echoat $b 第18頁 空頁
at $b echoat $b 第19頁 [1;36m迷霧之島之其他區域             [37m(    boss19     )[0m
at $b echoat $b 第20頁 空頁
at $b echoat $b 第21頁 空頁
at $b echoat $b 第22頁 [1;33m銀龍海灣暨地層魔域             [37m(    boss22     )[0m
def_rembch null-tar
~
>room_defun b01~
at $b echoat $b .		[1;36m[史卡拉貝及週邊區域 I][0m
if is_qmark($b) & adv_book_1_main 100001
	at $b echoat $b [1;35m執法者                  史卡拉貝       已擊敗[0m
else
	at $b echoat $b [1;32m執法者                  史卡拉貝       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100002
	at $b echoat $b [1;35m終結者                  史卡拉貝       已擊敗[0m
else
	at $b echoat $b [1;32m終結者                  史卡拉貝       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100004
	at $b echoat $b [1;35m黛妮雅                  史卡拉貝       已擊敗[0m
else
	at $b echoat $b [1;32m黛妮雅                  史卡拉貝       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100008
	at $b echoat $b [1;35m蘭斯洛特                史卡拉貝       已擊敗[0m
else
	at $b echoat $b [1;32m蘭斯洛特                史卡拉貝       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b02~
at $b echoat $b .		[1;36m[史卡拉貝及週邊區域 II][0m
if is_qmark($b) & adv_book_1_main 200001
	at $b echoat $b [1;35m小惡鬼外圍軍團長        密登尼森林     已擊敗[0m
else
	at $b echoat $b [1;32m小惡鬼外圍軍團長        密登尼森林     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200002
	at $b echoat $b [1;35m小惡鬼禁衛軍團長        密登尼森林     已擊敗[0m
else
	at $b echoat $b [1;32m小惡鬼禁衛軍團長        密登尼森林     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200004
	at $b echoat $b [1;35m小惡鬼總參謀歐巴桑      密登尼森林     已擊敗[0m
else
	at $b echoat $b [1;32m小惡鬼總參謀歐巴桑      密登尼森林     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200256
	at $b echoat $b [1;35m小惡鬼弩砲              密登尼森林     已擊敗[0m
else
	at $b echoat $b [1;32m小惡鬼弩砲              密登尼森林     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200512
	at $b echoat $b [1;35m小惡鬼攻城槌            密登尼森林     已擊敗[0m
else
	at $b echoat $b [1;32m小惡鬼攻城槌            密登尼森林     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 201024
	at $b echoat $b [1;35m小惡鬼投石機            密登尼森林     已擊敗[0m
else
	at $b echoat $b [1;32m小惡鬼投石機            密登尼森林     未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b03~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b04~
at $b echoat $b .		[1;36m[史卡拉貝西邊區域 I][0m
if is_qmark($b) & adv_book_1_main 100016
	at $b echoat $b [1;35m黃金劍客                銀蛇武士堡     已擊敗[0m
else
	at $b echoat $b [1;32m黃金劍客                銀蛇武士堡     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100032
	at $b echoat $b [1;35m銀蛇堡堡主              銀蛇武士堡     已擊敗[0m
else
	at $b echoat $b [1;32m銀蛇堡堡主              銀蛇武士堡     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100064
	at $b echoat $b [1;35m劍客維達                銀蛇武士堡     已擊敗[0m
else
	at $b echoat $b [1;32m劍客維達                銀蛇武士堡     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200032
	at $b echoat $b [1;35m巨魔領導者毆拉毆拉      古沼地         已擊敗[0m
else
	at $b echoat $b [1;32m巨魔領導者毆拉毆拉      古沼地         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100128
	at $b echoat $b [1;35m瑞貞豪克                綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m瑞貞豪克                綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100256
	at $b echoat $b [1;35m綠樹衛隊隊長            綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m綠樹衛隊隊長            綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 100512
	at $b echoat $b [1;35m梅黛拉                  綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m梅黛拉                  綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 101024
	at $b echoat $b [1;35m卡秋殺                  綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m卡秋殺                  綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 102048
	at $b echoat $b [1;35m綠樹衛隊副隊長          綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m綠樹衛隊副隊長          綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 104096
	at $b echoat $b [1;35m退休大臣                綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m退休大臣                綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 108192
	at $b echoat $b [1;35m賈霸                    綠樹王國       已擊敗[0m
else
	at $b echoat $b [1;32m賈霸                    綠樹王國       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 200001
	at $b echoat $b [1;35m緹雅瑪特                龍塔           已擊敗[0m
else
	at $b echoat $b [1;32m緹雅瑪特                龍塔           未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 200002
	at $b echoat $b [1;35m魔法女王                魔法高塔       已擊敗[0m
else
	at $b echoat $b [1;32m魔法女王                魔法高塔       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b05~
at $b echoat $b .		[1;36m[史卡拉貝西邊區域 II][0m
if is_qmark($b) & adv_book_1_west 100001
	at $b echoat $b [1;35m羅胡公爵                羅胡城堡       已擊敗[0m
else
	at $b echoat $b [1;32m羅胡公爵                羅胡城堡       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100002
	at $b echoat $b [1;35m羅胡夫人                羅胡城堡       已擊敗[0m
else
	at $b echoat $b [1;32m羅胡夫人                羅胡城堡       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100004
	at $b echoat $b [1;35m變種白龍                羅胡城堡       已擊敗[0m
else
	at $b echoat $b [1;32m變種白龍                羅胡城堡       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100008
	at $b echoat $b [1;35m幽冥巨獸                幽冥深淵       已擊敗[0m
else
	at $b echoat $b [1;32m幽冥巨獸                幽冥深淵       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100016
	at $b echoat $b [1;35m闇影惡魔1               幽冥深淵       已擊敗[0m
else
	at $b echoat $b [1;32m闇影惡魔1               幽冥深淵       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100032
	at $b echoat $b [1;35m闇影惡魔2               幽冥深淵       已擊敗[0m
else
	at $b echoat $b [1;32m闇影惡魔2               幽冥深淵       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100064
	at $b echoat $b [1;35m闇影之王                幽冥深淵       已擊敗[0m
else
	at $b echoat $b [1;32m闇影之王                幽冥深淵       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 116384
	at $b echoat $b [1;35m古代飛龍                禁斷之塔       已擊敗[0m
else
	at $b echoat $b [1;32m古代飛龍                禁斷之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 132768
	at $b echoat $b [1;35m闇黑之靈                禁斷之塔       已擊敗[0m
else
	at $b echoat $b [1;32m闇黑之靈                禁斷之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 200004
	at $b echoat $b [1;35m惡魔騎士                禁斷之塔       已擊敗[0m
else
	at $b echoat $b [1;32m惡魔騎士                禁斷之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100128
	at $b echoat $b [1;35m黑魔龍                  炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m黑魔龍                  炎紅之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100256
	at $b echoat $b [1;35m歐迦                    炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m歐迦                    炎紅之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 100512
	at $b echoat $b [1;35m歐絲                    炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m歐絲                    炎紅之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 101024
	at $b echoat $b [1;35m克里特                  炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m克里特                  炎紅之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 102048
	at $b echoat $b [1;35m緹斯．娜                炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m緹斯．娜                炎紅之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 104096
	at $b echoat $b [1;35m法培                    炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m法培                    炎紅之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_west 108192
	at $b echoat $b [1;35m黑水晶龍傀儡            炎紅之塔       已擊敗[0m
else
	at $b echoat $b [1;32m黑水晶龍傀儡            炎紅之塔       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b06~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b07~
at $b echoat $b .		[1;36m[史卡拉貝東邊區域及大沙漠區 I][0m
if is_qmark($b) & adv_book_1_main 200008
	at $b echoat $b [1;35m古托邦                  精靈之森       已擊敗[0m
else
	at $b echoat $b [1;32m古托邦                  精靈之森       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200016
	at $b echoat $b [1;35m古拉邦                  精靈之森       已擊敗[0m
else
	at $b echoat $b [1;32m古拉邦                  精靈之森       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200064
	at $b echoat $b [1;35m甘道夫                  賽伐斯拖普     已擊敗[0m
else
	at $b echoat $b [1;32m甘道夫                  賽伐斯拖普     未擊敗[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100016
	at $b echoat $b [1;35m莫斯提瑪                泰洛斯         已擊敗[0m
else
	at $b echoat $b [1;32m莫斯提瑪                泰洛斯         未擊敗[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100032
	at $b echoat $b [1;35m瑪門                    泰洛斯         已擊敗[0m
else
	at $b echoat $b [1;32m瑪門                    泰洛斯         未擊敗[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100064
	at $b echoat $b [1;35m黑鱗邪龍赫瑞析          泰洛斯         已擊敗[0m
else
	at $b echoat $b [1;32m黑鱗邪龍赫瑞析          泰洛斯         未擊敗[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100128
	at $b echoat $b [1;35m屍妖萊諾                泰洛斯         已擊敗[0m
else
	at $b echoat $b [1;32m屍妖萊諾                泰洛斯         未擊敗[0m
endif
if is_qmark($b) & thalos_darkforce_quest 100256
	at $b echoat $b [1;35m青銅巨人塔洛斯          泰洛斯         已擊敗[0m
else
	at $b echoat $b [1;32m青銅巨人塔洛斯          泰洛斯         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100512
	at $b echoat $b [1;35m亞殤洞穴巨人            深土城         已擊敗[0m
else
	at $b echoat $b [1;32m亞殤洞穴巨人            深土城         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100008
	at $b echoat $b [1;35m皇女阿狄蕾              深土城         已擊敗[0m
else
	at $b echoat $b [1;32m皇女阿狄蕾              深土城         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100016
	at $b echoat $b [1;35m拉席卡                  深土城         已擊敗[0m
else
	at $b echoat $b [1;32m拉席卡                  深土城         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100032
	at $b echoat $b [1;35m索爾格林姆              深土城         已擊敗[0m
else
	at $b echoat $b [1;32m索爾格林姆              深土城         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100064
	at $b echoat $b [1;35m梅法拉                  深土城         已擊敗[0m
else
	at $b echoat $b [1;32m梅法拉                  深土城         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100128
	at $b echoat $b [1;35m混沌九頭蛇              深土沼澤       已擊敗[0m
else
	at $b echoat $b [1;32m混沌九頭蛇              深土沼澤       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100256
	at $b echoat $b [1;35m萬古樹妖                深土沼澤       已擊敗[0m
else
	at $b echoat $b [1;32m萬古樹妖                深土沼澤       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 101024
	at $b echoat $b [1;35m雙足龍王                深土沼澤       已擊敗[0m
else
	at $b echoat $b [1;32m雙足龍王                深土沼澤       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b08~
at $b echoat $b .		[1;36m[史卡拉貝東邊區域及大沙漠區 II][0m
if is_qmark($b) & adv_book_1_east 200002
	at $b echoat $b [1;35m大惡魔                  永恆之塔       已擊敗[0m
else
	at $b echoat $b [1;32m大惡魔                  永恆之塔       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100001
	at $b echoat $b [1;35m雅克                    蠻荒森林       已擊敗[0m
else
	at $b echoat $b [1;32m雅克                    蠻荒森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100002
	at $b echoat $b [1;35m艾蓮娜                  蠻荒森林       已擊敗[0m
else
	at $b echoat $b [1;32m艾蓮娜                  蠻荒森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 100004
	at $b echoat $b [1;35m賽巴洛司                蠻荒森林       已擊敗[0m
else
	at $b echoat $b [1;32m賽巴洛司                蠻荒森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_east 200001
	at $b echoat $b [1;35m艾莉女爵                麥格城         已擊敗[0m
else
	at $b echoat $b [1;32m艾莉女爵                麥格城         未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b09~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b10~
at $b echoat $b .		[1;36m[平行世界以及外圍地區 I][0m
if is_qmark($b) & adv_book_1_other 100001
	at $b echoat $b [1;35m大不列顛英王            創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m大不列顛英王            創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100002
	at $b echoat $b [1;35m聖者                    創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m聖者                    創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100004
	at $b echoat $b [1;35m時光之神                創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m時光之神                創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100008
	at $b echoat $b [1;35m蒙丹                    創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m蒙丹                    創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100016
	at $b echoat $b [1;35m米娜                    創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m米娜                    創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100032
	at $b echoat $b [1;35m埃索德斯                創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m埃索德斯                創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100064
	at $b echoat $b [1;35m暗黑之王                創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m暗黑之王                創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 100128
	at $b echoat $b [1;35m守護者                  創世紀         已擊敗[0m
else
	at $b echoat $b [1;32m守護者                  創世紀         未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 200128
	at $b echoat $b [1;35m黑帝斯                  奧林帕斯山     已擊敗[0m
else
	at $b echoat $b [1;32m黑帝斯                  奧林帕斯山     未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 116384
	at $b echoat $b [1;35m吉斯王                  星界           已擊敗[0m
else
	at $b echoat $b [1;32m吉斯王                  星界           未擊敗[0m
endif
if is_qmark($b) & adv_book_1_main 132768
	at $b echoat $b [1;35m帕拉瑪斯                涅盤大地       已擊敗[0m
else
	at $b echoat $b [1;32m帕拉瑪斯                涅盤大地       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b11~
at $b echoat $b .		[1;36m[平行世界以及外圍地區 I][0m
if is_qmark($b) & adv_book_1_other 200001
	at $b echoat $b [1;35m雪后                    南極大地       已擊敗[0m
else
	at $b echoat $b [1;32m雪后                    南極大地       未擊敗[0m
endif
if is_qmark($b) & adv_book_1_other 200002
	at $b echoat $b [1;35m柳敬明                  清河鎮         已擊敗[0m
else
	at $b echoat $b [1;32m柳敬明                  清河鎮         未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b12~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b13~
at $b echoat $b .		[1;36m[迷霧之島主區][0m
if is_qmark($b) & adv_book_2_1 100032
	at $b echoat $b [1;35m卡瑪斯　                楓紅林         已擊敗[0m
else
	at $b echoat $b [1;32m卡瑪斯　                楓紅林         未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b14~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b15~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b16~
at $b echoat $b .		[1;36m[迷霧之島奈圖王國][0m
if is_qmark($b) & adv_book_2_1 100064
	at $b echoat $b [1;35m神聖屠殺者              高地　         已擊敗[0m
else
	at $b echoat $b [1;32m神聖屠殺者              高地　         未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100128
	at $b echoat $b [1;35m吸血鬼王　　            鬼鎮　　       已擊敗[0m
else
	at $b echoat $b [1;32m吸血鬼王　　            鬼鎮　　       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100256
	at $b echoat $b [1;35m死靈之王　　            鬼鎮　　       已擊敗[0m
else
	at $b echoat $b [1;32m死靈之王　　            鬼鎮　　       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100512
	at $b echoat $b [1;35m王者惡魔                鬼鎮　　       已擊敗[0m
else
	at $b echoat $b [1;32m王者惡魔                鬼鎮　　       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 101024
	at $b echoat $b [1;35m黑暗之王　　            鬼鎮　　       已擊敗[0m
else
	at $b echoat $b [1;32m黑暗之王　　            鬼鎮　　       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 102048
	at $b echoat $b [1;35m墮天使長　              鬼鎮　　       已擊敗[0m
else
	at $b echoat $b [1;32m墮天使長　              鬼鎮　　       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 104096
	at $b echoat $b [1;35m行影者　                鬼鎮　　       已擊敗[0m
else
	at $b echoat $b [1;32m行影者　                鬼鎮　         未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 108192
	at $b echoat $b [1;35m寒霜惡魔　              鬼鎮　         已擊敗[0m
else
	at $b echoat $b [1;32m寒霜惡魔　              鬼鎮　         未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 200001
	at $b echoat $b [1;35m煉獄惡魔                鬼鎮　         已擊敗[0m
else
	at $b echoat $b [1;32m煉獄惡魔                鬼鎮　         未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 200002
	at $b echoat $b [1;35m四翼墮天使              鬼鎮　         已擊敗[0m
else
	at $b echoat $b [1;32m四翼墮天使              鬼鎮　         未擊敗[0m
endif
if is_qmark($b) & wander_quest 200001
	at $b echoat $b [1;35m利牙殺手　　            幻惑迷林       已擊敗[0m
else
	at $b echoat $b [1;32m利牙殺手　　            幻惑迷林       未擊敗[0m
endif
if is_qmark($b) & wander_quest 200004
	at $b echoat $b [1;35m猛毒咬噬　　            幻惑迷林       已擊敗[0m
else
	at $b echoat $b [1;32m猛毒咬噬　　            幻惑迷林       未擊敗[0m
endif
if is_qmark($b) & wander_quest 200016
	at $b echoat $b [1;35m王者之冠                幻惑迷林       已擊敗[0m
else
	at $b echoat $b [1;32m王者之冠                幻惑迷林       未擊敗[0m
endif
if is_qmark($b) & wander_quest 200064
	at $b echoat $b [1;35m暴躁外皮　　            幻惑迷林       已擊敗[0m
else
	at $b echoat $b [1;32m暴躁外皮　　            幻惑迷林       未擊敗[0m
endif
if is_qmark($b) & wander_quest 200256
	at $b echoat $b [1;35m血腥之拳　              幻惑迷林       已擊敗[0m
else
	at $b echoat $b [1;32m血腥之拳　              幻惑迷林       未擊敗[0m
endif
if is_qmark($b) & wander_quest 201024
	at $b echoat $b [1;35m黃金盔甲　              幻惑迷林       已擊敗[0m
else
	at $b echoat $b [1;32m黃金盔甲　              幻惑迷林       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b17~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b18~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b19~
at $b echoat $b .		[1;36m[迷霧之島之其他區域][0m
if is_qmark($b) & adv_book_2_1 100001
	at $b echoat $b [1;35m遠古惡魔                遠古森林       已擊敗[0m
else
	at $b echoat $b [1;32m遠古惡魔                遠古森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100002
	at $b echoat $b [1;35m泰坦巨人首領            遠古森林       已擊敗[0m
else
	at $b echoat $b [1;32m泰坦巨人首領            遠古森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100004
	at $b echoat $b [1;35m梅杜莎  　　            遠古森林       已擊敗[0m
else
	at $b echoat $b [1;32m梅杜莎  　　            遠古森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100008
	at $b echoat $b [1;35m遠古末日巨獸            遠古森林       已擊敗[0m
else
	at $b echoat $b [1;32m遠古末日巨獸            遠古森林       未擊敗[0m
endif
if is_qmark($b) & adv_book_2_1 100016
	at $b echoat $b [1;35m巨型獅鷲獸　            遠古森林       已擊敗[0m
else
	at $b echoat $b [1;32m巨型獅鷲獸　            遠古森林       未擊敗[0m
endif
if is_qmark($b) & sealevil_quest 300001
	at $b echoat $b [1;35m尼勒　　　　            封魔洞窟       已擊敗[0m
else
	at $b echoat $b [1;32m尼勒　　　　            封魔洞窟       未擊敗[0m
endif
if is_qmark($b) & sealevil_quest 300002
	at $b echoat $b [1;35m莫羅比斯　　            封魔洞窟       已擊敗[0m
else
	at $b echoat $b [1;32m莫羅比斯　　            封魔洞窟       未擊敗[0m
endif
if is_qmark($b) & sealevil_quest 300004
	at $b echoat $b [1;35m艾梅蒂　                封魔洞窟       已擊敗[0m
else
	at $b echoat $b [1;32m艾梅蒂　                封魔洞窟       未擊敗[0m
endif
if is_qmark($b) & sealevil_quest 300008
	at $b echoat $b [1;35m奧法　　　　            封魔洞窟       已擊敗[0m
else
	at $b echoat $b [1;32m奧法　　　　            封魔洞窟       未擊敗[0m
endif
if is_qmark($b) & sealevil_quest 300016
	at $b echoat $b [1;35m迪瑞爾　　              封魔洞窟       已擊敗[0m
else
	at $b echoat $b [1;32m迪瑞爾　　              封魔洞窟       未擊敗[0m
endif
def_rembch null-tar
~
>room_defun b20~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b21~
at $b echoat $b 空頁
def_rembch null-tar
~
>room_defun b22~
at $b echoat $b .		[1;36m[銀龍海灣暨地層魔域][0m
if is_qmark($b) & dragonbay_quest_list 100008
	at $b echoat $b [1;35m亞殤巨人王　            地層魔域       已擊敗[0m
else
	at $b echoat $b [1;32m亞殤巨人王　            地層魔域       未擊敗[0m
endif
if is_qmark($b) & dragonbay_quest_list 100016
	at $b echoat $b [1;35m魔眼蜘蛛皇后            魔眼叢林       已擊敗[0m
else
	at $b echoat $b [1;32m魔眼蜘蛛皇后            魔眼叢林       未擊敗[0m
endif
if is_qmark($b) & dragonbay_quest_list 100032
	at $b echoat $b [1;35m牛魔王        　        地層魔域       已擊敗[0m
else
	at $b echoat $b [1;32m牛魔王        　        地層魔域       未擊敗[0m
endif
if is_qmark($b) & dragonbay_quest_list 100064
	at $b echoat $b [1;35m嗜血將軍　　            地層魔域       已擊敗[0m
else
	at $b echoat $b [1;32m嗜血將軍　　            地層魔域       未擊敗[0m
endif
if is_qmark($b) & dragonbay_quest_list 100128
	at $b echoat $b [1;35m萬影之王　              地層魔域       已擊敗[0m
else
	at $b echoat $b [1;32m萬影之王　              地層魔域       未擊敗[0m
endif
if is_qmark($b) & dragonbay_quest_list 100256
	at $b echoat $b [1;35m女妖之后　              地層魔域       已擊敗[0m
else
	at $b echoat $b [1;32m女妖之后　              地層魔域       未擊敗[0m
endif
def_rembch null-tar
~
|
