==============================================================================
【MiHaJong】          Japanese Mahjong with a lot of local rules implemented
                   Copyright (c) 2008-2012 MihailJP, Part of rights reserved
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

	　Windows XP/Vista/7、Direct X 9 以降
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


■ 既知の問題点(通信対戦移植ベータ版)

	・対戦開始時に通信対戦とは無関係のソケットエラーが表示される。
	・チャットが使用できない。
	・落ち戻りの処理が未実装。
	・その他通信絡みの問題があるかもしれない。


■ 既知の問題点(長期積み残し)

	・OSの設定で左右のボタンを入れ替えているとマウスボタンで正常に打牌できない。
	・ポン聴または吃聴の直後、画面下の待ち牌表示に変なものが表示される。
	・上家が親のときサイコロの上に文字が表示される。


■ MihailJPの開発環境

	・OS：Windows 7 Home Premium 32bit (Service Pack 1)
	・物理メモリ：4GB（3.2GB使用可能）
	・CPU：Intel Core i5 760 2.80GHz（場合により2.66GHzにダウンクロック）
	・開発環境：Hot Soup Processor ver3.3、Visual C++ 2010 Express
	・画像編集ツール：GIMP 2.6
	・テキストエディタ：Mery、Notepad++、サクラエディタ、Kate
	・Gitクライアント：Cygwin GIT


■ ビルドに別途必要なもの

	・Hot Soup Processor 本体 (http://hsp.tv/)
	・p2b.dll (http://www.geocities.jp/anachronism128/)
	・hmm.dll (http://hsp.tv/make/tool3.html)
	・hspcksum.dll (https://github.com/MihailJP/HSPCkSum/downloads)
	・l_tooltips.as (http://lhsp.s206.xrea.com/hsp_object6.html)
	・mod_customizebutton.hsp (http://hspdev-wiki.net/?Module/mod_customizebutton_v2)
	・cHspComp (http://lldev.jp/others/freeware.html#chspcomp)
	・Lua スタティックライブラリ (http://luabinaries.sourceforge.net/download.html)
	・Ruby 処理系 (http://www.ruby-lang.org/ja/downloads/)


■ ライセンスについて

	　license.txt をご覧ください。


■ バージョン履歴

	　history.txt をご覧ください。


==============================================================================
                                                                       Finis
==============================================================================
