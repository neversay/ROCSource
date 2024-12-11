>defun_prog wander_rest~
if isfight($i)
	break
else
        mpat 5 mpforcer here c 'full heal' $i 40
        mpat 5 mpforcer here c 'det inv' $i 40
        mpat 5 mpforcer here c 'det hid' $i 40
        rest
endif
~
|