#pragma once

#include "../sound/sound.h"

namespace sound {
namespace util {

void bgmload(unsigned ID, const char* filename, bool looped); /* BGM読み込み */
void soundload(unsigned ID, const char* filename, bool looped); /* サウンド読み込み */
void bgmstop(); /* BGM停止 */
void bgmplay(unsigned ID); /* ＢＧＭ再生 */
void setvolume(); /* 音量設定を反映 */

}
}
