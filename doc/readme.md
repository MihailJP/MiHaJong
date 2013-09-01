MiHaJong: Japanese Mahjong with a lot of local rules implemented
================================================================
Copyright (c) 2008-2013 MihailJP, Part of rights reserved


はじめに
--------
　このゲームは、ごく普通の麻雀ゲームです。
 
　日本で一般的に行なわれているリーチ麻雀で行なわれますが、「喰い断の有無」「二翻縛りの有無」といったローカルルールや、
「大車輪」「八連荘」「十三不塔」などローカル役などの設定項目が多数存在します。
設定できる項目の中には「西中島南方」「双龍争珠」「覇王鞭」「青天井ルール」と言った、ソフトウェアとして実装されるのが稀なものも含まれます。

　また、牌譜出力機能もあり、インターネットを経由した通信対戦も可能です。

　なお、麻雀の具体的な遊び方については別途麻雀入門書などをご覧ください。


動作環境
--------
- Windows版はWindows XP/Vista/7、Direct X 9 (June 2010) 以降
- Linux版はX11(xlib)、OpenGL(GLX)、OpenAL、libpng、libluaが必要
- HTML形式、XML形式の牌譜を閲覧する場合は別途フォントのインストールが必要


インストールのしかた
--------------------
　ダウンロードしたアーカイブファイルの中身を適当な場所に解凍してください。
Vista 以降をお使いの場合で C:\Program Files 以下にインストールした場合、自動的に認識し設定や牌譜のファイルの出力先を再設定します
(Program Filesへのインストール自体は管理者権限が必要ですが、プレイ時には管理者権限は不要です)。

　Linux版は `./configure --with-boost-regex=boost_regex && make && sudo make install` して下さい。

**※GCCのstd::regexは壊れているため、boost::regexを使用して下さい。**


アンインストールのしかた
------------------------
　レジストリは使わないので、インストールした先をフォルダーごと削除するだけです。
牌譜を取っておきたい場合は haifu フォルダーをバックアップしておきます。
 
　Vista 以降をお使いの場合で C:\Program Files 以下にインストールした場合、
設定と牌譜は C:\Users\\(ユーザー名)\AppData\Roaming\MiHaJong 以下に出力されますので、必要のない方はこれも削除します。

　なお、Linux版では設定ファイルや牌譜は /home/(ユーザー名)/.mihajong 以下に出力されます。


あそびかた
----------
　[playing.md](playing.md) をご覧ください。


何かあったら
------------
　バグレポートやパッチ、役満和了ったなどの報告は、次の連絡先にどうぞ。
- メール： mihailjp@gmail.com
- Twitter： @MihailJP

　また、バグレポートや要望は[開発室(Wiki)](http://www14.atwiki.jp/mihajong/)でも受け付けています。

　最新の開発版は[GitHubのリポジトリ](https://github.com/MihailJP/MiHaJong/)で公開されています。


MihailJPの開発環境
------------------
- OS：Windows 7 Home Premium 32bit (Service Pack 1)
- OS：OpenSUSE 12.3 x86_64
- 物理メモリ：4GB（32bit OSでは3.2GB使用可能）
- CPU：Intel Core i5 760 2.80GHz（場合により2.66GHzにダウンクロック）
- 開発環境：Visual C++ 2010 Express (Windows)、GCC 4.7 (Linux)
- 画像編集ツール：GIMP 2.6
- テキストエディタ：Mery、Notepad++、Kate
- Gitクライアント：Cygwin GIT (Windows)、GIT (Linux)


ビルドに別途必要なもの
----------------------
- [liblua](http://www.lua.org/download.html) (Visual C++の場合はスタティックリンクします)
- [Ruby 処理系](http://www.ruby-lang.org/ja/downloads/)
- [Python 処理系](http://www.python.jp/download/)
- [markdown_py](https://pypi.python.org/pypi/Markdown)
- [libogg/libvorbis](http://www.xiph.org/downloads/) (Visual C++の場合はスタティックリンクします)
- [GuruGuruSMF](http://gurugurusmf.migmig.net/) (Windowsのみ)
- [boost::regex](http://www.boost.org/) (GCCはstd::regexが壊れているため必要。Visual C++の場合は不要)
- [OpenAL](http://connect.creativelabs.com/openal/default.aspx) (Linuxのみ)
- [libpng](http://www.libpng.org/pub/png/libpng.html) (Linuxのみ)


Linux版の制限
-------------
- チャットウィンドウや名前設定で日本語の入力ができません。
- フルスクリーンにできません。
- 効果音が鳴りません。
- BGMが鳴りません。


ライセンスについて
------------------
　[license.md](./license.md) をご覧ください。


バージョン履歴
--------------
　[history.md](./history.md) をご覧ください。


------------------------------------------------------------------------------

_Finis_
