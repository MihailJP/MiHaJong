set VCCDIR="C:\Program Files\Microsoft Visual Studio 10.0"
call %VCCDIR%\Common7\Tools\vsvars32.bat
msbuild mjlib.sln /p:Configuration=Release
copy Release\mjlib.dll ..\mihajong\
