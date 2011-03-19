/*=============================
 * HSP麻雀クライアントMiHaJong
 *  [プラグインの読み込み等]
 *=============================
 */

/* コンパイル時のオプション */
#ifdef SANMAS
#packopt name "mihassnm"
#define global ALLSANMA
#define global SANMAT
#undef SANMA
#undef SANMA4
#else
	#ifdef SANMA4
	#packopt name "mihaysnm"
	#define global SANMAX
	#define global ALLSANMA
	#undef SANMA
	#else
		#ifdef SANMA
		#packopt name "mihasanm"
		#define global SANMAT
		#define global SANMAX
		#define global ALLSANMA
		#else
		#packopt name "mihajong"
		#endif
	#endif
#endif
#packopt hide 1
#packopt type 0
#cmpopt ppout 1
#cmpopt optcode 1
#cmpopt optinfo 1
#cmpopt varname 1
#runtime "hsp3mt"

#define WITH_MT19937AR

/* 拡張機能の登録 */
#include "p2b.as"
#include "hmm.as"
#include "hspsock.as"
#include "llmod3.hsp"
#include "obj.hsp"
#include "l_tooltips.as"
#define tooltip(%1,%2) AddToolTips (%1)+1,%2
#define vanish clrobj:SetToolTips
#define global STATBOX 1
#ifdef WITH_MT19937AR
	#undef rnd
	#undef randomize
	#module redef
	#defcfunc rnd int p1
	#include "hspda.as"
		z=0: rndf_geti z, p1
		return z
	#deffunc randomize
		rndf_int -1
		return
	#global
#endif
#include "hspmath.as"
#include "hsplzmax.as"

#ifdef NOWAIT
	#undef await
	#define global await(%1) await@hsp 0
#endif
