@echo off
del !.tx~
glasscdr.exe e !test.txt !256free.txt !.txt >nul
glasscdr d !.txt !.tx~
fc !test.txt !.tx~
