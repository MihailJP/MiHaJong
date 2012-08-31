set HSPPATH="C:\Program Files\hsp321"
ruby auxtools\rulesyms.rb
cd mjcore
call build.bat
cd ..\mihajong
call build.bat
cd ..
call package.bat
