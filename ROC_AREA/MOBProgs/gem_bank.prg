>talk_prog p gem-bank~
tell $n 這是針對持有大量魔力寶石卻沒地方放的冒險者所加開的服務。
tell $n 只要持有契約就能在各地提供魔力寶石服務的銀行儲存及提領。
tell $n 開戶的價格為 100000，每顆魔力寶石儲存的價格為 500。
tell $n 每個戶頭能儲存最高 9999 個。
tell $n 每次交易能夠以 1 , 5 , 10 , 50 為單位進行存入或領出。
tell $n 只要告訴我以下的內容，就能直能進行交易。
tell $n 存入數量分別為：gem1 , gem5 , gem10 , gem50。
tell $n 領出數量分別為：rgem1 , rgem5 , rgem10 , rgem50。
tell $n 另外請注意，本行會將存入倉庫的寶石作均質化，所以領出的寶石效益可能下降。
tell $n 如需要開戶(gem-account)可以直接跟我說。
~
>talk_prog p gem-account~
if goldamt($n) >= 100000
	mpgold $n -100000
	mpoload 3012 $n 1 target
	mpechoat $n $I給了你魔力寶石契約(contract of magic gem).
	tell $n 請妥善保管您的契約，遺失將不會補發。
	tell $n 如想再次詢問詳細使用方法可以直接問我(gem-bank)。
else
	mpechoat $n $I苦笑著說：「對不起，你沒有足夠的契約費用。」
endif
~
>talk_prog p gem1~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 500
	mpechoat $n $I苦笑著說：「對不起，你的錢不夠請人保管魔力寶石。」
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 1
		mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石。」
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_1 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p gem5~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 2500
	mpechoat $n $I苦笑著說：「對不起，你的錢不夠請人保管這數量的魔力寶石。」
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 5
		mpechoat $n $I苦笑著說：「對不起，你身上沒有足夠的魔力寶石。」
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_5 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p gem10~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 5000
	mpechoat $n $I苦笑著說：「對不起，你的錢不夠請人保管這數量的魔力寶石。」
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 10
		mpechoat $n $I苦笑著說：「對不起，你身上沒有足夠的魔力寶石。」
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_10 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p gem50~
if is_qmark($n) == gembanking
	break
endif
if goldamt($n) < 25000
	mpechoat $n $I苦笑著說：「對不起，你的錢不夠請人保管這數量的魔力寶石。」
	break
endif
if is_inv_obj($n) == 3012
	if carrycount($n , 23) < 50
		mpechoat $n $I苦笑著說：「對不起，你身上沒有足夠的魔力寶石。」
		break
	else
		mpforcer here stun $n 2
		mpadd_qmark $n gembanking 2 0 0 0 0 1
		mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
		mpforce $n storecheck_50 'magic_alc gem_alc contract_alc'
	endif
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p rgem1~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 1
or canweight($n) < 3
	mpechoat $n $I苦笑著說：「你身上塞不下更多東西了。」
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I從後面的庫房將足夠數量的魔力寶石(magic gem)交給你。
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_1 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p rgem5~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 5
or canweight($n) < 15
	mpechoat $n $I苦笑著說：「你身上塞不下更多東西了。」
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I從後面的庫房將足夠數量的魔力寶石(magic gem)交給你。
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_5 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p rgem10~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 10
or canweight($n) < 30
	mpechoat $n $I苦笑著說：「你身上塞不下更多東西了。」
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I從後面的庫房將足夠數量的魔力寶石(magic gem)交給你。
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_10 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
>talk_prog p rgem50~
if is_qmark($n) == gembanking
	break
endif
if cancarry($n) < 50
or canweight($n) < 150
	mpechoat $n $I苦笑著說：「你身上塞不下更多東西了。」
	break
endif
if is_inv_obj($n) == 3012
	mpforcer here stun $n 2
	mpechoat $n $I從後面的庫房將足夠數量的魔力寶石(magic gem)交給你。
	mpadd_qmark $n gembanking 2 0 0 0 0 1
	mpforce $n mpforceo 'magic_alc gem_alc contract_alc' odef_rembch $i
	mpforce $n rececheck_50 'magic_alc gem_alc contract_alc'
else
	mpechoat $n $I苦笑著說：「對不起，你身上沒有魔力寶石契約，如需使用本服務請開戶(gem-account)。」
endif
~
|