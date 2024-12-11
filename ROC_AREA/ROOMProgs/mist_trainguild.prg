>room_enter 15 100~
if mobcount(4552) != 0
or mobcount(4553) != 0
or mobcount(4554) != 0
or mobcount(4555) != 0
or mobcount(4556) != 0
or mobcount(4557) != 0
or mobcount(4558) != 0
or mobcount(4559) != 0
or mobcount(4560) != 0
	break
endif
if isnpc($n)
	break
endif
if is_qmark($n) != mist_training
	echoat $n 遠方傳來一陣聲響「你的訓練已經結束！」
	transfer $n 4571
	break
endif
if npcnum($) > 4
	break
endif
if pcnum($) > 1
	break
endif
if is_qmark($n) == mist_training 100001
	mload 4552
	force warrior mpoload 4504
	force warrior mpoload 4520
	force warrior mpat 5 wear all
	mload 4553
	force magician mpoload 4585
	force magician mpoload 4564
	force magician mpat 5 wear all
	mload 4554
	force cleric mpoload 4585
	force cleric mpoload 4564
	force cleric mpat 5 wear all
	force magician mpgroup warrior self 0
	force cleric mpgroup warrior self 0
	force warrior protect cleric
	force warrior mpkill $n
	break
endif
if is_qmark($n) == mist_training 100002
	mload 4555
	force warrior mpoload 4504
	force warrior mpoload 4520
	force warrior mpat 5 wear all
	mload 4556
	force magician mpoload 4585
	force magician mpoload 4564
	force magician mpat 5 wear all
	mload 4557
	force cleric mpoload 4585
	force cleric mpoload 4564
	force cleric mpat 5 wear all
	force magician mpgroup warrior self 0
	force cleric mpgroup warrior self 0
	force warrior protect cleric
	force warrior mpkill $n
endif
if is_qmark($n) == mist_training 100003
	mload 4558
	force warrior mpoload 4504
	force warrior mpoload 4520
	force warrior mpat 5 wear all
	mload 4559
	force magician mpoload 4585
	force magician mpoload 4564
	force magician mpat 5 wear all
	mload 4560
	force cleric mpoload 4585
	force cleric mpoload 4564
	force cleric mpat 5 wear all
	force magician mpgroup warrior self 0
	force cleric mpgroup warrior self 0
	force warrior protect cleric
	force warrior mpkill $n
	break
endif
~
|
