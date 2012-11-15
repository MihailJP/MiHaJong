@rem 
@rem =====================================================
@rem MiHaJong 自動ビルド用バッチファイル
@rem =====================================================
@rem 
@rem 環境変数 %HSPPATH%に、予めHSPのパスを設定の上、
@rem cHspCompをインストールしてください。
@rem http://lldev.jp/others/freeware.html#chspcomp
@rem 

for %%i in ( mihajong.hsp mihasanm.hsp mihaysnm.hsp mihassnm.hsp ) do %HSPPATH%\chspcomp.exe /m %CD%\%%i
