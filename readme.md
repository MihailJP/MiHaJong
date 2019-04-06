MiHaJong: Japanese Mahjong with a lot of local rules implemented
================================================================
Copyright (c) 2008-2013, 2019 MihailJP


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
- Windows版はWindows 7/10、Direct X 9 (June 2010) 以降
- Linux版はX11(xlib)、OpenGL(GLX)、OpenAL、libpng、libluaが必要


インストールのしかた
--------------------
　ダウンロードしたアーカイブファイルの中身を適当な場所に解凍してください。
Vista 以降をお使いの場合で C:\Program Files 以下にインストールした場合、自動的に認識し設定や牌譜のファイルの出力先を再設定します
(Program Filesへのインストール自体は管理者権限が必要ですが、プレイ時には管理者権限は不要です)。

　Linux版は ./configure && make && sudo make install して下さい。


アンインストールのしかた
------------------------
　レジストリは使わないので、インストールした先をフォルダーごと削除するだけです。
牌譜を取っておきたい場合は haifu フォルダーをバックアップしておきます。
 
　Vista 以降をお使いの場合で C:\Program Files 以下にインストールした場合、
設定と牌譜は C:\Users\(ユーザー名)\AppData\Roaming\MiHaJong 以下に出力されますので、必要のない方はこれも削除します。

　なお、Linux版では設定ファイルや牌譜は /home/(ユーザー名)/.mihajong 以下に出力されます。


あそびかた
----------
　[playing.md](doc/playing.md) をご覧ください。


中国麻将について
----------------
　MiHaJong 2.1.0 より、中国麻将版の実行ファイル・コアDLLを同梱しています。

　中国麻将とは、中国政府公認の国際ルールにより行われる麻雀です。
MiHaJongでは、基本的に2006年版のルールに準拠していますが、なにかおかしい点があるかもしれません。

　中国麻将の詳しい遊び方については、次のサイトが参考になるでしょう。

* Wikipedia: 中国麻雀 (http://ja.wikipedia.org/wiki/%E4%B8%AD%E5%9B%BD%E9%BA%BB%E9%9B%80)
* 東方キャラによる中国麻将の紹介ページ（仮） (http://www.green.dti.ne.jp/maisan/c_mahjong/)
* 中国式麻将（中国式麻雀） ルールと戦略 (http://www.flowstones.com/mj/Cmj/China-index.html)


何かあったら
------------
　バグレポートやパッチ、役満和了ったなどの報告は、次の連絡先にどうぞ。
- メール： mihailjp@gmail.com
- Twitter： @MihailJP

　また、バグレポートや要望は開発室(Wiki)でも受け付けています。
- URL: http://www14.atwiki.jp/mihajong/

　最新の開発版はGitHubのリポジトリで公開されています。
- URL: https://github.com/MihailJP/MiHaJong/

MihailJPの開発環境
------------------
- OS：Windows 10 Home 64bit
- OS：OpenSUSE Leap 15.0 x86_64
- 物理メモリ：12GB
- CPU：Intel Core i7 860
- GPU：GeForce GTX 950
- 開発環境：Visual Studio Community 2017 (Windows)、GCC 4.7 (Linux)
- 画像編集ツール：GIMP 2.8
- テキストエディタ：Mery、Notepad++、Kate
- Gitクライアント：GitHub Desktop (Windows)、GIT (Linux)


ビルドに別途必要なもの
----------------------
- liblua (http://www.lua.org/download.html) (Visual C++の場合はスタティックリンクします)
- Ruby 処理系 (http://www.ruby-lang.org/ja/downloads/)
- Python 処理系 (http://www.python.jp/download/)
- markdown_py (https://pypi.python.org/pypi/Markdown)
- libogg/libvorbis (http://www.xiph.org/downloads/) (Visual C++の場合はスタティックリンクします)
- GuruGuruSMF (http://gurugurusmf.migmig.net/) (Windowsのみ)
- OpenAL (http://connect.creativelabs.com/openal/default.aspx) (Linuxのみ)
- libpng (http://www.libpng.org/pub/png/libpng.html) (Linuxのみ)


Linux版の制限
-------------
- フルスクリーンにできません。
- 効果音が鳴りません。
- BGMが鳴りません。
- ジョイスティックは使えません。


ライセンスについて
------------------
　ソースコードはMITライセンスでリリースされます。
その他については、ドキュメントに記載されています。
[license.md](doc/license.md) をご覧ください。


バージョン履歴
--------------
　[history.md](doc/history.md) をご覧ください。


------------------------------------------------------------------------------

Finis
