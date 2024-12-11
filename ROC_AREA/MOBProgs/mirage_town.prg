>fight_prog 100~
if ispc($n)
        if is_qmark($n) == mirage_enemy
                say 走吧，別破壞這裡的寧靜。
                c 'faerie fog'
                mptransfer $n 401
        else
                mpadd_all_qmark $n mirage_enemy -1 0 0 0 0 1
                say 走吧，別破壞這裡的寧靜。
                c 'faerie fog'
                mptransfer $n 401
        endif
endif
~
>death_prog 100~
mpecho $I的身影逐漸模糊，慢慢的消失在空氣中。
mpadd_all_qmark $n mirage_enemy -1 0 0 0 0 1
mpgoto 59400
mppurge self
~
|
