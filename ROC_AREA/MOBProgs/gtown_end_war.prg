>defun_prog gt_war_ending~
mpdis_magic self blindness
mpat 37000 c inf
mpat 37000 mpforce mayor-gtown yell 感謝你們的幫忙！敵人已經全部消滅了！
mpat 37000 mpforce mayor-gtown yell 請回來尖塔頂層吧，全員到齊後跟我說一聲(talk mayor come)
mpat 37000 mptransfer mayor-gtown 37061
mpat 37000 mptransfer kasshu-gtown 37061
mpgoto seal-gtown
if is_qmark(seal-gtown) != gtown_war_level
	if rand(33)
		mpadd_qmark seal-gtown gtown_war_level -1 120 0 0 0 1
	else
		mpadd_qmark seal-gtown gtown_war_level -1 60 0 0 0 1
	endif
else
	if rand(33)
		mpmod_qmark seal-gtown gtown_war_level v0 + 120
	else
		mpmod_qmark seal-gtown gtown_war_level v0 + 60
	endif
endif
mpgoto 37061
if is_qmark(mayor-gtown) > gtown_death_war_death 100018
	mpdel_qmark mayor-gtown gtown_death_war_death
	mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_one
	mpgoto $n
	if is_qmark(seal-gtown) == gwar_lv 100000
		mpgreward exp $n +36 10000
		mpgreward skexp $n +36 500
		mpgreward merit $n +36 10
	endif
	if is_qmark(seal-gtown) == gwar_lv 100001
		mpgreward exp $n +36 15000
		mpgreward skexp $n +36 1000
		mpgreward merit $n +36 15
	endif
	if is_qmark(seal-gtown) == gwar_lv 100002
		mpgreward exp $n +36 20000
		mpgreward skexp $n +36 2000
		mpgreward merit $n +36 20
	endif
	if is_qmark(seal-gtown) == gwar_lv 100003
		mpgreward exp $n +36 25000
		mpgreward skexp $n +36 3000
		mpgreward merit $n +36 25
	endif
	if is_qmark(seal-gtown) == gwar_lv 100004
		mpgreward exp $n +36 30000
		mpgreward skexp $n +36 4000
		mpgreward merit $n +36 35
	endif
	if is_qmark(seal-gtown) == gwar_lv 100005
		mpgreward exp $n +36 35000
		mpgreward skexp $n +36 5000
		mpgreward merit $n +36 45
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100018
	if is_qmark(mayor-gtown) > gtown_death_war_death 100016
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_two
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 15000
			mpgreward skexp $n +36 600
			mpgreward merit $n +36 15
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 20000
			mpgreward skexp $n +36 1300
			mpgreward merit $n +36 25
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 25000
			mpgreward skexp $n +36 2400
			mpgreward merit $n +36 40
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 30000
			mpgreward skexp $n +36 3500
			mpgreward merit $n +36 50
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 35000
			mpgreward skexp $n +36 4500
			mpgreward merit $n +36 70
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 40000
			mpgreward skexp $n +36 6000
			mpgreward merit $n +36 80
		endif
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100016
	if is_qmark(mayor-gtown) > gtown_death_war_death 100013
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_three
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 20000
			mpgreward skexp $n +36 700
			mpgreward merit $n +36 20
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 25000
			mpgreward skexp $n +36 1500
			mpgreward merit $n +36 30
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 30000
			mpgreward skexp $n +36 2800
			mpgreward merit $n +36 50
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 35000
			mpgreward skexp $n +36 4000
			mpgreward merit $n +36 70
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 40000
			mpgreward skexp $n +36 5000
			mpgreward merit $n +36 90
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 45000
			mpgreward skexp $n +36 7000
			mpgreward merit $n +36 110
		endif
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100013
	if is_qmark(mayor-gtown) > gtown_death_war_death 100011
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_four
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 25000
			mpgreward skexp $n +36 800
			mpgreward merit $n +36 25
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 30000
			mpgreward skexp $n +36 1700
			mpgreward merit $n +36 35
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 35000
			mpgreward skexp $n +36 3200
			mpgreward merit $n +36 60
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 40000
			mpgreward skexp $n +36 4500
			mpgreward merit $n +36 85
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 45000
			mpgreward skexp $n +36 5500
			mpgreward merit $n +36 110
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 50000
			mpgreward skexp $n +36 8000
			mpgreward merit $n +36 135
		endif
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100011
	if is_qmark(mayor-gtown) > gtown_death_war_death 100008
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_five
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 30000
			mpgreward skexp $n +36 900
			mpgreward merit $n +36 30
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 35000
			mpgreward skexp $n +36 1900
			mpgreward merit $n +36 45
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 40000
			mpgreward skexp $n +36 3600
			mpgreward merit $n +36 75
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 45000
			mpgreward skexp $n +36 5000
			mpgreward merit $n +36 105
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 50000
			mpgreward skexp $n +36 6000
			mpgreward merit $n +36 135
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 55000
			mpgreward skexp $n +36 9000
			mpgreward merit $n +36 165
		endif
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100008
	if is_qmark(mayor-gtown) > gtown_death_war_death 100006
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_six
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 35000
			mpgreward skexp $n +36 1000
			mpgreward merit $n +36 35
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 40000
			mpgreward skexp $n +36 2100
			mpgreward merit $n +36 50
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 45000
			mpgreward skexp $n +36 4000
			mpgreward merit $n +36 85
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 50000
			mpgreward skexp $n +36 5500
			mpgreward merit $n +36 120
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 55000
			mpgreward skexp $n +36 6500
			mpgreward merit $n +36 155
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 60000
			mpgreward skexp $n +36 10000
			mpgreward merit $n +36 190
		endif
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100006
	if is_qmark(mayor-gtown) > gtown_death_war_death 100003
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_seven
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 40000
			mpgreward skexp $n +36 1100
			mpgreward merit $n +36 40
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 45000
			mpgreward skexp $n +36 2300
			mpgreward merit $n +36 60
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 50000
			mpgreward skexp $n +36 4400
			mpgreward merit $n +36 100
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 55000
			mpgreward skexp $n +36 6000
			mpgreward merit $n +36 140
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 60000
			mpgreward skexp $n +36 7000
			mpgreward merit $n +36 180
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 65000
			mpgreward skexp $n +36 11000
			mpgreward merit $n +36 220
		endif
	endif
endif
if is_qmark(mayor-gtown) <= gtown_death_war_death 100003
	if is_qmark(mayor-gtown) > gtown_death_war_death 100001
		mpdel_qmark mayor-gtown gtown_death_war_death
		mpat 37061 mpadd_qmark mayor-gtown gtown_death_mayor_end_eight
		mpgoto $n
		if is_qmark(seal-gtown) == gwar_lv 100000
			mpgreward exp $n +36 45000
			mpgreward skexp $n +36 1200
			mpgreward merit $n +36 45
		endif
		if is_qmark(seal-gtown) == gwar_lv 100001
			mpgreward exp $n +36 50000
			mpgreward skexp $n +36 2500
			mpgreward merit $n +36 65
		endif
		if is_qmark(seal-gtown) == gwar_lv 100002
			mpgreward exp $n +36 55000
			mpgreward skexp $n +36 4600
			mpgreward merit $n +36 110
		endif
		if is_qmark(seal-gtown) == gwar_lv 100003
			mpgreward exp $n +36 60000
			mpgreward skexp $n +36 6500
			mpgreward merit $n +36 155
		endif
		if is_qmark(seal-gtown) == gwar_lv 100004
			mpgreward exp $n +36 65000
			mpgreward skexp $n +36 7500
			mpgreward merit $n +36 200
		endif
		if is_qmark(seal-gtown) == gwar_lv 100005
			mpgreward exp $n +36 70000
			mpgreward skexp $n +36 12000
			mpgreward merit $n +36 250
		endif
	endif
endif
~
|