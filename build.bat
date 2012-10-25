set HSPPATH="C:\Program Files\hsp321"
set BASEDIR=%CD%
ruby auxtools\rulesyms.rb
cd %BASEDIR%\mjcore
call build.bat
cd %BASEDIR%\mihajong
call build.bat
cd %BASEDIR%
call package.bat
