@echo off
echo clean up...


if exist %~p0Debug rd /s /q %~p0Debug

for /F  %%d in ('dir /b /s /AD Debug') do rd /s /q %%d
for /F  %%d in ('dir /b /s /AD Release') do rd /s /q %%d
for /F  %%d in ('dir /b /AD') do if exist %%d\x64 rd /s /q %%d\x64
for /F  %%d in ('dir /b /AD') do if exist %%d\doxygen.tmp rd /s /q %%d\doxygen.tmp
