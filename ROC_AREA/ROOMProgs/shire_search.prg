>room_enter 127 60~
 if is_qmark($n) == shire_haton_chicken 200005
   echoat $n �t�a�ǨӤF�@�}���L���B�B�n�C
 endif
~
>room_command search chicken~
 if is_qmark($n) == shire_haton_chicken 200005
   echo �@���ιʹʪ������q���v�B�]�F�X�ӡC
   mod_qmark $n shire_haton_chicken v1 = 6
   oload 1120 viclv $n floor
 endif
~
|
