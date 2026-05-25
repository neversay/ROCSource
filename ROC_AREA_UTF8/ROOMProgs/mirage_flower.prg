>room_enter 127 100~
if ispc($n)
	if hour($) == 0
	or hour($) == 1
	or hour($) == 2
	or hour($) == 3
	or hour($) == 4
	or hour($) == 21
	or hour($) == 22
	or hour($) == 23
		if day($) == 7
		or day($) == 14
		or day($) == 21
		or day($) == 28
		or day($) == 35
			if roomvalue($0) == 0
				echo [1;35m紫琉璃百合(Purple Lazurite Lily)在月光的照射下閃閃發光。[m
				if rand(10)
					echoat $n 一陣想法油然而生，你想去摘花(pluck lily)。
				endif
			endif
		endif
	endif
endif
~
>room_command pluck lily~
if hour($) == 0
or hour($) == 1
or hour($) == 2
or hour($) == 3
or hour($) == 4
or hour($) == 21
or hour($) == 22
or hour($) == 23
	if day($) == 7
	or day($) == 14
	or day($) == 21
	or day($) == 28
	or day($) == 35
		if roomvalue($0) == 0
			echoat $n 你摘下了一朵[1;35m紫琉璃百合(Purple Lazurite Lily)[m
			echoaround $n $N摘下了一朵[1;35m紫琉璃百合(Purple Lazurite Lily)[m
			oload 59206 30 $n
			modrvt v0 = 1
		endif
	endif
endif
~
|