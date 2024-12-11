c:
cd \Envy22\area
if exist c:\envy22\log\envy.log del c:\envy22\log\envy.log
if exist SHUTDOWN.TXT del SHUTDOWN.TXT
:while1
sleep 10
envy_32 > c:\envy22\log\envy.log
if not exist SHUTDOWN.TXT goto :while1
del SHUTDOWN.TXT
