>room_enter 127 60~
 if is_qmark($n) == shire_haton_chicken 200005
   echoat $n 暗地傳來了一陣輕微的咕咕聲。
 endif
~
>room_command search chicken~
 if is_qmark($n) == shire_haton_chicken 200005
   echo 一隻肥嘟嘟的母雞從陰影處跑了出來。
   mod_qmark $n shire_haton_chicken v1 = 6
   oload 1120 viclv $n floor
 endif
~
|
