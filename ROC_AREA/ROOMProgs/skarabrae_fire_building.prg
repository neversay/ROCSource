>room_enter 127 100~
if roomvalue($0) == 0
	break
endif
if roomvalue($0) <= 3
	echoat $n [1;36m�o�̧e�{���L�l�ˡC[0m
	break
endif
if roomvalue($0) <= 6
	echoat $n [1;32m�o�̧e�{�Y���l�ˡA�h�B�Ыΰ_���C[0m
	break
endif
if roomvalue($0) <= 9
	echoat $n [1;33m�o�̧e�{�D�`�Y�����l�ˡA�|�P���J�@�������C[0m
	break
else
	echoat $n [1;31m�o�̧e�{�������A�A���������ŹM�G�|�P�C[0m
	break
endif
~
|