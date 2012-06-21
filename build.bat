set HSPPATH="C:\Program Files\hsp321"
cd mjlib
call build.bat
cd ..\mihajong
call build.bat
cd ..
call package.bat
