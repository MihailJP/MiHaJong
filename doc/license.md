MiHaJong ライセンス
===================
Copyright (c) 2008-2013 MihailJP, Part of rights reserved


ライセンスについて
------------------
　このソフトウェアはどなたでも無償でご使用いただけますが、著作権を放棄するものではありません。

　ソフトウェア全体としては、次のライセンスで提供されるものとします。使用した素材などの利用条件は後述してあります。

> Copyright (c) 2008-2013 MihailJP, Part of rights reserved.
> 
> 　ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。
> 
> - ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。
> - バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。
> - 本ソフトウェアで使用されている画像、音声、その他のコンポーネントに追加の使用条件が設定されているものがある場合は、その条件にも従うこと。
> - 書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、
>   MihailJPの名前またはコントリビューターの名前を使用してはならない。
> 
> **　本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、
> 明示黙示を問わず、商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、
> またそれに限定されない、いかなる保証もありません。
> 著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、
> かつ責任の根拠が契約であるか厳格責任であるか（過失その他の）不法行為であるかを問わず、
> 仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
> （代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
> 直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。**


使用した牌画データの元データについて
------------------------------------
　Gutenberg Laboで配布されている「GL-MahjongTile」を元にしています。
　※Version 2.0より牌画データを全面差し替えしました。

> These fonts are free softwares.
> 
> Unlimited permission is granted to use, copy, and distribute it, with or without modification,
> either commercially and noncommercially.
> 
> THESE FONTS ARE PROVIDED "AS IS" WITHOUT WARRANTY.
>
> これらのフォントはフリー（自由な）ソフトウエアです。
> あらゆる改変の有無に関わらず、また商業的な利用であっても、自由にご利用、
> 複製、再配布することができますが、全て無保証とさせていただきます。

　参照：http://gutenberg.sourceforge.jp/ja/license.html


使用した効果音のデータについて
------------------------------
　「ザ・マッチメイカァズ」で配布されているフリー素材を使用しております。

> ＜＜素材利用規約＞＞
> 
> ＊ここでの”フリー素材”という言葉は、利用規約の範囲内であれば各データを無料かつ自由に使用できるという意味で掲げております。
> 著作権の放棄は しておりません。  
> 
> ＊何らかの創作物の一部として使用せずに 素材その物のみを無断で転載・配布する事は禁止いたします。
> 素材データとしての転載・再配布を行いたい場合は事前にご相談下さい。
> 
> 　ここの素材を加工・改造した物を”素材として”広く配布する事もできません（配布を行いたい場合は事前にご相談下さい）
> 
> ＊素材ファイルへの直リンクは一切禁止いたします

参照：http://osabisi.sakura.ne.jp/m2/


Perlからの移植コードについて
----------------------------
　ゲームの本質ではない部分で、Perlからの移植コードを使用しています。移植し
たモジュールとライセンスは次の通りです。

### Astro::MoonPhase ###

> #### LICENCE ####
> This program is in the public domain: "Do what thou wilt shall be the whole of the law".
>
> #### AUTHORS ####
> The moontool.c Release 2.0:
> 
>     A Moon for the Sun
>     Designed and implemented by John Walker in December 1987,
>     revised and updated in February of 1988.
>
> Initial Perl transcription:
> 
>     Raino Pikkarainen, 1998
>     raino.pikkarainen@saunalahti.fi
>
> The moontool.c Release 2.4:
> 
>     Major enhancements by Ron Hitchens, 1989
>
> Revisions:
> 
>     Brett Hamilton http://simple.be/
>     Bug fix, 2003
>     Second transcription and bugfixes, 2004
>     
>     Christopher J. Madsen http://www.cjmweb.net/
>     Added phaselist function, March 2007

参照：http://search.cpan.org/~brett/Astro-MoonPhase/MoonPhase.pm

### Astro::Sunrise ###

> Here is the copyright information provided by Paul Schlyer:
>
> Written as DAYLEN.C, 1989-08-16
> 
> Modified to SUNRISET.C, 1992-12-01
> 
> (c) Paul Schlyter, 1989, 1992
> 
> Released to the public domain by Paul Schlyter, December 1992
>
> Permission is hereby granted, free of charge, to any person
> obtaining a copy of this software and associated documentation files
> (the "Software"), to deal in the Software without restriction,
> including without limitation the rights to use, copy, modify, merge,
> publish, distribute, sublicense, and/or sell copies of the Software,
> and to permit persons to whom the Software is furnished to do so,
> subject to the following conditions:
>
> The above copyright notice and this permission notice shall be
> included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
> EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
> NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
> CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
> TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
> SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

参照：http://search.cpan.org/~rkhill/Astro-Sunrise-0.91/Sunrise.pm


Luaインタプリタについて
-----------------------
　AIの思考ルーチンに、Luaスクリプトのインタプリタを使用しています。ライセンスは次の通りです。

> Copyright (C) 1994-2012 Lua.org, PUC-Rio.
>
> Permission is hereby granted, free of charge, to any person obtaining a
> copy of this software and associated documentation files (the
> "Software"), to deal in the Software without restriction, including
> without limitation the rights to use, copy, modify, merge, publish,
> distribute, sublicense, and/or sell copies of the Software, and to
> permit persons to whom the Software is furnished to do so, subject to
> the following conditions:
>
> The above copyright notice and this permission notice shall be included
> in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
> OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
> IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
> CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
> TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
> SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

参照：http://www.lua.org/manual/5.2/readme.html#license


libogg/libvorbisについて
------------------------
　サウンド再生ルーチンに、libogg/libvorbisのコードを使用しています。ライセンスは次の通りです。

### libogg (libogg_static.lib) ###

> Copyright (c) 2002, Xiph.org Foundation
>
> Redistribution and use in source and binary forms, with or without
> modification, are permitted provided that the following conditions
> are met:
>
> - Redistributions of source code must retain the above copyright
> notice, this list of conditions and the following disclaimer.
>
> - Redistributions in binary form must reproduce the above copyright
> notice, this list of conditions and the following disclaimer in the
> documentation and/or other materials provided with the distribution.
>
> - Neither the name of the Xiph.org Foundation nor the names of its
> contributors may be used to endorse or promote products derived from
> this software without specific prior written permission.
>
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
> ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
> LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
> FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
> FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
> INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
> BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
> LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
> CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
> LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
> ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
> POSSIBILITY OF SUCH DAMAGE.

### libvorbis (libvorbis_static.lib、libvorbisfile_static.lib) ###

> Copyright (c) 2002-2008 Xiph.org Foundation
>
> Redistribution and use in source and binary forms, with or without
> modification, are permitted provided that the following conditions
> are met:
>
> - Redistributions of source code must retain the above copyright
> notice, this list of conditions and the following disclaimer.
>
> - Redistributions in binary form must reproduce the above copyright
> notice, this list of conditions and the following disclaimer in the
> documentation and/or other materials provided with the distribution.
>
> - Neither the name of the Xiph.org Foundation nor the names of its
> contributors may be used to endorse or promote products derived from
> this software without specific prior written permission.
>
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
> ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
> LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
> FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
> FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
> INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
> BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
> LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
> CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
> LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
> ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
> POSSIBILITY OF SUCH DAMAGE.


GuruGuruSMF4.dllについて（Windows版のみ）
----------------------------------------
　Ver1.9.0以降、MIDI再生ルーチンは GuruGuruSMF4.dll に依存しています。ライセンスは次の通りです。

> GuruGuruSMF4 (C)2003-2011 卯如
>
> フリーウェアです。著作権は放棄されていません。  
> 無保証です。  
> dll/hpiでリンクする場合、使用の報告、表示の義務もありません。そのままノーロイヤリティで商用利用も可能です。  
> 修正BSDライセンスによるソースコードの公開も予定してます。


LZMA SDKについて
----------------
　圧縮データの読み込みに、LZMA SDKのコードを使用しています。ライセンスは次
の通りです。

> LZMA SDK is written and placed in the public domain by Igor Pavlov.


------------------------------------------------------------------------------

_Finis_
