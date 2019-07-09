@echo off
for /F %%a in ('dir /s /b secmain.exe') do (
    cd %%~pa
    secmain.exe /debug
)
