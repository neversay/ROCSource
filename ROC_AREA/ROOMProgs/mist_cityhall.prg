>room_enter 63 100~
if isnpc($n)
	break
endif
if mobcount(4561) > 8
	break
endif
if hour($) < 8
or hour($) > 17
	echo ��A��J�o�̪��ɭԡA���M��ӥ�ĳ�|�T�_ĵ�ܪ��n���C
	echo �u�@��ĳ�|���u�é��M�X�{�F....�C
	mload 4561
	force council mpdef_rembch $n
	force council follow $n
	mload 4561
	force council mpdef_rembch $n
	force council mpgroup 2.council self 1 0
	force council follow $n
endif
~
|