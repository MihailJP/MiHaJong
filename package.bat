rmdir /s /q release

mkdir release
mkdir release\mihajong
copy mihajong\*.exe release\mihajong
del /q release\mihajong\logger.exe
copy mihajong\*.dll release\mihajong
copy mihajong\readme.txt release\mihajong
copy mihajong\license.txt release\mihajong
copy mihajong\history.txt release\mihajong
copy mihajong\playing.txt release\mihajong
mkdir release\mihajong\bgm
mkdir release\mihajong\ai
copy mihajong\ai\*.* release\mihajong\ai
mkdir release\mihajong\haifu
copy mihajong\haifu\haifu.css release\mihajong\haifu
mkdir release\mihajong\img
copy mihajong\img\*.* release\mihajong\img
mkdir release\mihajong\sound
copy mihajong\sound\*.* release\mihajong\sound
call archive_source.bat
copy source.zip release\mihajong
del /q mihajong.zip
cd release
"C:\Program Files\7-zip\7z.exe" a -mx=9 ..\mihajong.zip mihajong
cd ..
