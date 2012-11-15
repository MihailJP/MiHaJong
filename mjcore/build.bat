set VCCDIR="C:\Program Files\Microsoft Visual Studio 10.0"
call %VCCDIR%\Common7\Tools\vsvars32.bat
msbuild mjcore.sln /p:Configuration=Release
copy Release\mjcore.dll ..\mihajong\
copy Release\astro.dll ..\mihajong\
copy Release\socket.dll ..\mihajong\
