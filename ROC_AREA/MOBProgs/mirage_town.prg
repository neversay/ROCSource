>fight_prog 100~
if ispc($n)
        if is_qmark($n) == mirage_enemy
                say ���a�A�O�}�a�o�̪����R�C
                c 'faerie fog'
                mptransfer $n 401
        else
                mpadd_all_qmark $n mirage_enemy -1 0 0 0 0 1
                say ���a�A�O�}�a�o�̪����R�C
                c 'faerie fog'
                mptransfer $n 401
        endif
endif
~
>death_prog 100~
mpecho $I�����v�v���ҽk�A�C�C�������b�Ů𤤡C
mpadd_all_qmark $n mirage_enemy -1 0 0 0 0 1
mpgoto 59400
mppurge self
~
|
