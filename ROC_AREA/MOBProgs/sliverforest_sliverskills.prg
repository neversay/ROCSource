>defun_prog sliverforest_queen_call~
if rand(25)
	if rand(25)
		mpmload 72001
		mpgroup self sliver-01 1 1
	else
		if rand(33)
			mpmload 72002
			mpgroup self sliver-2 1 1
		else
			if rand(50)
				mpmload 72003
				mpgroup self sliver-3 1 1
			else
				mpmload 72004
				mpgroup self sliver-4 1 1
			endif
		endif
	endif
else
	if rand(33)
		if rand(25)
			mpmload 72005
			mpgroup self sliver-5 1 1
		else
			if rand(33)
				mpmload 72006
				mpgroup self sliver-6 1 1
			else
				if rand(50)
					mpmload 72007
					mpgroup self sliver-7 1 1
				else
					mpmload 72008
					mpgroup self sliver-8 1 1
				endif
			endif
		endif
	else
		if rand(50)
			if rand(25)
				mpmload 72009
				mpgroup self sliver-9 1 1
			else
				if rand(33)
					mpmload 72010
					mpgroup self sliver-10 1 1
				else
					if rand(50)
						mpmload 72011
						mpgroup self sliver-11 1 1
					else
						mpmload 72012
						mpgroup self sliver-12 1 1
					endif
				endif
			endif
		else
			if rand(25)
				mpmload 72013
				mpgroup self sliver-13 1 1
			else
				if rand(33)
					mpmload 72014
					mpgroup self sliver-14 1 1
				else
					if rand(50)
						mpmload 72015
						mpgroup self sliver-15 1 1
					else
						mpmload 72016
						mpgroup self sliver-16 1 1
					endif
				endif
			endif
		endif
	endif
endif
~
>rand_prog 100~
if insameroom(sliver-01)
	mpmodify special | 1024
else
	mpmodify special r 1024
endif
if insameroom(sliver-02)
	if rand(1)
		echo �����O�|������
	endif
endif
if insameroom(sliver-3)
	mpmodify aff | 134217728
else
	mpmodify aff r 134217728
endif
if insameroom(sliver-4)
	mpmodify act | 16777216
else
	mpmodify act r 16777216
endif
if insameroom(sliver-5)
	mpdis_magic $i 'pass door'
	mpdis_magic $i 'breathe water'
	mpforcer here add_affect $i 'pass door' 0000 -1 42 300 0
	mpforcer here add_affect $i 'breathe water' 0000 -1 41 200 0
else
	mpdis_magic $i 'pass door'
	mpdis_magic $i 'breathe water'
endif
if insameroom(sliver-6)
	mpdis_magic $i mute
	mpdis_magic $i 'seal magic'
	mpforcer here add_affect $i mute 0000 -1 43 200 0
	mpforcer here add_affect $i 'seal magic' 0000 -1 44 350 0
else
	mpdis_magic $i mute
	mpdis_magic $i 'seal magic'
endif
if insameroom(sliver-7)
	mpmodify act | 2048
else
	mpmodify act r 2048
endif
if insameroom(sliver-9)
	mpmodify special | 2048
else
	mpmodify special r 2048
endif
if insameroom(sliver-16)
	mpmodify aff | 524288
	mpforcer here add_affect $i 'stone skin' 0000 -1 39 -400 0
else
	mpmodify aff r 524288
	mpdis_magic $i 'stone skin'
endif
if pcnum($) == 0
	mpdis_magic $i blindness
endif
~
>fight_prog 100~
wake
mpnowait
if rand(40)
	if rand(50)
		mpforcer here cast 'burning hands' $n 50
		mpnowait
	endif
	if rand(50)
		mpforcer here cast 'flamestrike' $n 50
		mpnowait
	endif
	if rand(10)
		mpecho �ǡ�ǡ�$I��Ū���A�I�s�P��[�J�ԧ��D
		mpcallout 1 mpcall sliverforest_queen_call $i
		mpcallout 1 mpecho �@��������ť��I��A�[�J�F�ԧ��A�ϥ��̾ԤO�j�W�D
	endif
endif
if rand(40)
	if rand(50)
		mpforcer here cast 'chill touch' $n 50
		mpnowait
	endif
	if rand(50)
		mpforcer here cast 'faerie fire' $n 50
		mpnowait
	endif
endif
if insameroom(sliver-01)
	mpmodify special | 1024
else
	mpmodify special r 1024
endif
if insameroom(sliver-2)
	if rand(1)
		echo �����O�|������
	endif
endif
if insameroom(sliver-3)
	mpmodify aff | 134217728
else
	mpmodify aff r 134217728
endif
if insameroom(sliver-4)
	mpmodify act | 16777216
else
	mpmodify act r 16777216
endif
if insameroom(sliver-5)
	mpdis_magic $i 'pass door'
	mpdis_magic $i 'breathe water'
	mpforcer here add_affect $i 'pass door' 0000 -1 42 300 0
	mpforcer here add_affect $i 'breathe water' 0000 -1 41 200 0
else
	mpdis_magic $i 'pass door'
	mpdis_magic $i 'breathe water'
endif
if insameroom(sliver-6)
	mpdis_magic $i mute
	mpdis_magic $i 'seal magic'
	mpforcer here add_affect $i mute 0000 -1 43 300 0
	mpforcer here add_affect $i 'seal magic' 0000 -1 44 350 0
else
	mpdis_magic $i mute
	mpdis_magic $i 'seal magic'
endif
if insameroom(sliver-7)
	mpmodify act | 2048
else
	mpmodify act r 2048
endif
if insameroom(sliver-9)
	mpmodify special | 2048
else
	mpmodify special r 2048
endif
if insameroom(sliver-16)
	mpmodify aff | 524288
	mpdis_magic $i 'stone skin'
	mpforcer here add_affect $i 'stone skin' 0000 -1 39 -400 0
else
	mpmodify aff r 524288
	mpdis_magic $i 'stone skin'
endif
if rand(35)
	if insameroom(sliver-9)
		mpforcer here modhmv $i 02.300 350 00000000
		mpecho $I���W���c�X�@�ǩ_�S���H�G�A�v¡�F���W���˶աC
	endif
endif
if rand(30)
	if insameroom(sliver-8)
		frenzy $n
		mpnowait
	endif
endif
if rand(35)
	if insameroom(sliver-10)
		mpforcer here cast 'star fire' $n 50
		mpnowait
		if rand(20)
			mpforcer here cast 'burning hands' $n 50
			mpnowait
		endif
		if rand(20)
			mpforcer here cast flamestrike $n 50
			mpnowait
		endif
		if rand(16)
			mpforcer here cast fireball $n 50
			mpnowait
		endif
	endif
endif
if rand(25)
	if insameroom(sliver-10)
		mpforcer here cast 'high explosive' $n 50
		mpnowait
		if rand(35)
			mpforcer here cast 'acid blast' $n 50
			mpnowait
		endif
		if rand(16)
			mpforcer here cast fireball $n 50
			mpnowait
		endif
	endif
endif
if insameroom(sliver-11)
	mpdis_magic $i 'detect magic'
	mpforcer here add_affect $i 'detect magic' 0000 0 43 +100 16
	skilldrv heavyhit $n
	skilldrv heavyhit $n
	skilldrv heavyhit $n
	if rand(20)
		skilldrv heavyhit $n
		if rand(20)
			skilldrv heavyhit $n
			if rand(20)
				skilldrv heavyhit $n
			endif
		endif
	endif
endif
if rand(15)
	if insameroom(sliver-12)
		mpechoat $r �A�ݵ�$I�ǲ����ʧ@�A�@�ѹ��c���Pı��M��W���Y......
		mpechoaround $r $I���M�@�P���̰ʰ_�ӡA���P��޲��������Aı�o���I����l�C
		mpdis_magic $r all
		mpforcer here add_affect $r curse 1000 2 18 -15 1024
		mpforcer here add_affect $r curse 1000 2 24 +14 1024
		mpforcer here add_affect $r curse 1000 2 35 +14 1024	
		mpforcer here add_affect $r 'faerie fire' 1000 2 17 +288
		mpforcer here add_affect $r weaken 1000 2 1 -2 0
		mpforcer here add_affect $r weaken 1000 2 43 -20 0
		mpforcer here add_affect $r poison 1000 2 5 -2 4096
		mpforcer here add_affect $r blindness 1000 2 18 -4 1
		mpforcer here add_affect $r mute 1000 2 0 0 8388608
		mpforcer here add_affect $r 'seal magic' 1000 2 0 0 16777216
		mpforcer here add_affect $r disease 1000 2 1 -1 1099511627776
		mpforcer here add_affect $r paralyze 1000 2 39 +200 268435456
	endif
endif
if rand(35)	
	if insameroom(sliver-13)
		mpgoto self
		backstab $n
		mpnowait
	endif
endif
if rand(20)
	if insameroom(sliver-15)
		mpechoat $r $I�������r�F�A�@�f�A���A�P���������l�C
		mpadd_qmark $r sliverforest_slivers_poison 20 10 0 0 0 1
		mpforcer here add_affect $r poison 1000 20 5 -2 4096
	endif
endif
if insameroom(sliver-16)
	skilldrv speedhit $n
	skilldrv speedhit $n
	if rand(20)
		skilldrv speedhit $n
		if rand(20)
			skilldrv speedhit $n
		endif
	endif
endif
if rand(20)
	if pcnum($) > 26
		mpecho �ǡ�ǡ�$I���褣�Ĳ��A��Ū���A�I�s�P��[�J�ԧ��D
		mpcallout 1 mpcall sliverforest_queen_call $i
		mpcallout 1 mpecho �@��������ť��I��A�[�J�F�ԧ��A�ϥ��̾ԤO�j�W�D
		if rand(33)
			mpcallout 5 mpcall sliverforest_queen_call $i
			mpcallout 5 mpecho ��h���������`�ۥs�n���ӥ[�J�ԧ��I�I
		endif
	endif
endif
if insameroom(sliver-9)
	mpdis_magic $i 'detect evil'
	mpforcer here add_affect $i 'detect evil' 0000 -1 44 200 0
else
	mpdis_magic $i 'detect evil'
endif
if insameroom(sliver-12)
	mpdis_magic $i 'detect poison'
	mpforcer here add_affect $i 'detect poison' 0000 -1 44 200 0
else
	mpdis_magic $i 'detect poison'
endif
if insameroom(sliver-3)
	mpdis_magic $i invis
	mpforcer here add_affect $i invis 0000 -1 44 200 0
else
	mpdis_magic $i invis
endif
if rand(25)
	if pcnum($) > 21
		if rand(50)
		skilldrv multihit $n
		endif
		if rand(50)
			mpforcer here cast 'burning hands' $n 50
			mpnowait
		endif
		if rand(50)
			mpforcer here cast 'flamestrike' $n 50
			mpnowait
		endif
	endif
endif
~
|
