@rem 
@rem =====================================================
@rem MiHaJong 自動ビルド用バッチファイル
@rem =====================================================
@rem 
@rem 環境変数 %HSPPATH%に、予めHSPのパスを設定の上、
@rem cHspCompをインストールしてください。
@rem http://lldev.jp/others/freeware.html#chspcomp
@rem 

%HSPPATH%\chspcomp.exe /m logger.hsp
copy logger.exe ..\mihajong
