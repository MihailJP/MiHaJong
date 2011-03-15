/*=============================
 * HSP麻雀クライアントMiHaJong
 *  [命令や関数のエイリアス]
 *=============================
 */

/* function.hsp */
#define global fatal(%1) debug_logging %1, __file__, __line__, 0
#define global error(%1) debug_logging %1, __file__, __line__, 1
#define global warn(%1) debug_logging %1, __file__, __line__, 2
#define global info(%1) debug_logging %1, __file__, __line__, 3
#define global debug(%1) debug_logging %1, __file__, __line__, 4
#define global trace(%1) debug_logging %1, __file__, __line__, 5
#define global borderedtxt2a(%1,%2,%3,%4,%5,%6,%7,%8,%9=0) borderedtxt2 %1, %2, %3, (255-(%4))/2+(%4), (255-(%5))/2+(%5), (255-(%6))/2+(%6), %7, %8, (%4)*4/5, (%5)*4/5, (%6)*4/5, %9
#define global borderedtxt2b(%1,%2,%3,%4,%5,%6,%7,%8,%9=0) borderedtxt2 %1, %2, %3, (255-(%4))/3+(%4), (255-(%5))/3+(%5), (255-(%6))/3+(%6), %7, %8, (%4), (%5), (%6), %9
#define global borderedtxt3a(%1,%2,%3,%4,%5,%6,%7,%8,%9=0,%10=1.0) borderedtxt3 %1, %2, %3, (255-(%4))/2+(%4), (255-(%5))/2+(%5), (255-(%6))/2+(%6), %7, %8, (%4)*4/5, (%5)*4/5, (%6)*4/5, %9, %10
#define global ctype ShimochaOf(%1) RelativePositionOf(%1, RELATIVE_SHIMOCHA)
#define global ctype ToimenOf(%1) RelativePositionOf(%1, RELATIVE_TOIMEN)
#define global ctype KamichaOf(%1) RelativePositionOf(%1, RELATIVE_KAMICHA)

/* sound.hsp */
#define global bgmload(%1,%2) filename=(%1): bgmid=(%2): gosub *_bgmload
#define global soundload(%1,%2) filename=(%1): soundid=(%2): gosub *_soundload
