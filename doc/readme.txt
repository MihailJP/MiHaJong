==============================================================================
【MiHaJong】          Japanese Mahjong with a lot of local rules implemented
                   Copyright (c) 2008-2013 MihailJP, Part of rights reserved
==============================================================================

■ はじめに

	　このゲームは、ごく普通の麻雀ゲームです。
	　日本で一般的に行なわれているリーチ麻雀で行なわれますが、「喰い断の有無」
	「二翻縛りの有無」といったローカルルールや、「大車輪」「八連荘」「十三不
	塔」などローカル役などの設定項目が多数存在します。設定できる項目の中には
	「西中島南方」「双龍争珠」「覇王鞭」「青天井ルール」と言った、ソフトウェア
	として実装されるのが稀なものも含まれます。
	　また、牌譜出力機能もあり、インターネットを経由した通信対戦も可能です。
	　なお、麻雀の具体的な遊び方については別途麻雀入門書などをご覧ください。


■ 動作環境

	　Windows XP/Vista/7、Direct X 9 (June 2010) 以降
	　HTML形式の牌譜を閲覧する場合は別途フォントのインストールが必要


■ インストールのしかた

	　ダウンロードしたアーカイブファイルの中身を適当な場所に解凍してください。
	Vista 以降をお使いの場合で C:\Program Files 以下にインストールした場合、自
	動的に認識し設定や牌譜のファイルの出力先を再設定します(Program Filesへのイ
	ンストール自体は管理者権限が必要ですが、プレイ時には管理者権限は不要です)。


■ アンインストールのしかた

	　レジストリは使わないので、インストールした先をフォルダーごと削除するだけ
	です。牌譜を取っておきたい場合は haifu フォルダーをバックアップしておきま
	す。
	　Vista 以降をお使いの場合で C:\Program Files 以下にインストールした場合、
	設定と牌譜は C:\Users\(ユーザー名)\AppData\Roaming\MiHaJong 以下に出力され
	ますので、必要のない方はこれも削除します。


■ あそびかた

	　playing.txt をご覧ください。


■ 何かあったら

	　バグレポートやパッチ、役満和了ったなどの報告は、次の連絡先にどうぞ。
	・メール： mihailjp@gmail.com
	・Twitter： @MihailJP
	
	　また、バグレポートや要望は開発室(Wiki)でも受け付けています。
	http://www14.atwiki.jp/mihajong/
	　また、最新の開発版はGitHubのリポジトリで公開されています。
	https://github.com/MihailJP/MiHaJong/


■ ベータ版の制限

	・通信対局が未実装。
	・チャットが未実装。
	・終了した時、稀にMiHaJongのプロセスが残ることがある。


■ MihailJPの開発環境

	・OS：Windows 7 Home Premium 32bit (Service Pack 1)
	・物理メモリ：4GB（3.2GB使用可能）
	・CPU：Intel Core i5 760 2.80GHz（場合により2.66GHzにダウンクロック）
	・開発環境：Visual C++ 2010 Express
	・画像編集ツール：GIMP 2.6
	・テキストエディタ：Mery、Notepad++、Kate
	・Gitクライアント：Cygwin GIT


■ ビルドに別途必要なもの

	・liblua (http://www.lua.org/download.html) ※
	・Ruby 処理系 (http://www.ruby-lang.org/ja/downloads/)
	・ruby-lzmaが使えない場合、 Python 処理系 (http://www.python.jp/download/)
	・libogg/libvorbis (http://www.xiph.org/downloads/) ※
	・GuruGuruSMF (http://gurugurusmf.migmig.net/)
	※Visual C++の場合はスタティックリンクします


■ ライセンスについて

	　license.txt をご覧ください。


■ バージョン履歴

	　history.txt をご覧ください。


==============================================================================
                                                                       Finis
==============================================================================
