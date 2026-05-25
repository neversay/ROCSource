>repop_prog 100~
  mpoload 50063 self viclv target
  mpoload 50002 self viclv target
  mpoload 50018 self viclv target
  mpoload 50019 self viclv target
  mpat 50000 wear all
~
>fight_prog 100~
  if rand(33)
      skilldrv speedhit
  else
      if rand(50)
          skilldrv heavyhit
      endif
  endif
~
|