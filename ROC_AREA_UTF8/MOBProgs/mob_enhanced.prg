>repop_prog 100~
mpadd_qmark self mob_enhanced -1 0 0 0 0 1
~
>fight_prog 100~
if pcnum($) >= 5
	if is_qmark($i) !& mob_enhanced 100001
		mpmod_qmark self mob_enhanced v0 | 1
		mpmodify amdr + 25
		mpmodify apdr + 50
		mpmodify mdr + 50
		mpmodify pdr + 50
		mpmodify dr + 10
		mpmodify pp + 5
	endif
endif
if pcnum($) >= 6
	if is_qmark($i) !& mob_enhanced 100002
		mpmod_qmark self mob_enhanced v0 | 2
		mpmodify amdr + 25
		mpmodify apdr + 50
		mpmodify mdr + 50
		mpmodify pdr + 50
		mpmodify dr + 10
		mpmodify pp + 5
	endif
endif
if pcnum($) >= 7
	if is_qmark($i) !& mob_enhanced 100004
		mpmod_qmark self mob_enhanced v0 | 4
		mpmodify amdr + 25
		mpmodify apdr + 50
		mpmodify mdr + 50
		mpmodify pdr + 50
		mpmodify dr + 10
		mpmodify pp + 5
	endif
endif
if pcnum($) >= 8
	if is_qmark($i) !& mob_enhanced 100008
		mpmod_qmark self mob_enhanced v0 | 8
		mpmodify amdr + 25
		mpmodify apdr + 50
		mpmodify mdr + 50
		mpmodify pdr + 50
		mpmodify dr + 10
		mpmodify pp + 5
	endif
endif
if pcnum($) >= 9
	if is_qmark($i) !& mob_enhanced 100016
		mpmod_qmark self mob_enhanced v0 | 16
		mpmodify amdr + 25
		mpmodify apdr + 50
		mpmodify mdr + 50
		mpmodify pdr + 50
		mpmodify dr + 10
		mpmodify pp + 5
	endif
endif
if pcnum($) >= 10
	if is_qmark($i) !& mob_enhanced 100032
		mpmod_qmark self mob_enhanced v0 | 32
		mpmodify amdr + 50
		mpmodify apdr + 80
		mpmodify mdr + 80
		mpmodify pdr + 80
		mpmodify dr + 20
		mpmodify pp + 10
	endif
endif
if pcnum($) >= 11
	if is_qmark($i) !& mob_enhanced 100064
		mpmod_qmark self mob_enhanced v0 | 64
		mpmodify amdr + 50
		mpmodify apdr + 80
		mpmodify mdr + 80
		mpmodify pdr + 80
		mpmodify dr + 20
		mpmodify pp + 10
	endif
endif
if pcnum($) >= 15
	if is_qmark($i) !& mob_enhanced 100128
		mpmod_qmark self mob_enhanced v0 | 128
		mpmodify amdr + 300
		mpmodify apdr + 300
		mpmodify mdr + 500
		mpmodify pdr + 500
		mpmodify dr + 100
		mpmodify pp + 40
	endif
endif
~
>tick_prog 100~
if isfight($i)
	break
else
	if is_qmark($i) & mob_enhanced 100001
		mpmodify pp - 5
		mpmodify amdr - 25
		mpmodify apdr - 50
		mpmodify mdr - 50
		mpmodify pdr - 50
		mpmodify dr - 10
		mpmod_qmark self mob_enhanced v0 r 1
	endif
	if is_qmark($i) & mob_enhanced 100002
		mpmodify pp - 5
		mpmodify amdr - 25
		mpmodify apdr - 50
		mpmodify mdr - 50
		mpmodify pdr - 50
		mpmodify dr - 10
		mpmod_qmark self mob_enhanced v0 r 2
	endif
	if is_qmark($i) & mob_enhanced 100004
		mpmodify pp - 5
		mpmodify amdr - 25
		mpmodify apdr - 50
		mpmodify mdr - 50
		mpmodify pdr - 50
		mpmodify dr - 10
		mpmod_qmark self mob_enhanced v0 r 4
	endif
	if is_qmark($i) & mob_enhanced 100008
		mpmodify pp - 5
		mpmodify amdr - 25
		mpmodify apdr - 50
		mpmodify mdr - 50
		mpmodify pdr - 50
		mpmodify dr - 10
		mpmod_qmark self mob_enhanced v0 r 8
	endif
	if is_qmark($i) & mob_enhanced 100016
		mpmodify pp - 5
		mpmodify amdr - 25
		mpmodify apdr - 50
		mpmodify mdr - 50
		mpmodify pdr - 50
		mpmodify dr - 10
		mpmod_qmark self mob_enhanced v0 r 16
	endif
	if is_qmark($i) & mob_enhanced 100032
		mpmodify pp - 10
		mpmodify amdr - 50
		mpmodify apdr - 80
		mpmodify mdr - 80
		mpmodify pdr - 80
		mpmodify dr - 20
		mpmod_qmark self mob_enhanced v0 r 32
	endif
	if is_qmark($i) & mob_enhanced 100064
		mpmodify pp - 10
		mpmodify amdr - 50
		mpmodify apdr - 80
		mpmodify mdr - 80
		mpmodify pdr - 80
		mpmodify dr - 20
		mpmod_qmark self mob_enhanced v0 r 64
	endif
	if is_qmark($i) & mob_enhanced 100128
		mpmodify pp - 40
		mpmodify amdr - 300
		mpmodify apdr - 300
		mpmodify mdr - 500
		mpmodify pdr - 500
		mpmodify dr - 100
		mpmod_qmark self mob_enhanced v0 r 128
	endif
endif
~
|