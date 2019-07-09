@echo off
echo.
echo %0: deleting CdePkg from Build and from FV
for /F %%d in ('dir /s /b /AD build\cde*') do rd /s /q %%d 
ping 127.0.0.0 -n 4 > NUL
