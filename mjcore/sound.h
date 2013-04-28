#pragma once

#include "../sound/sound.h"

namespace sound {
namespace util {

void bgmload(unsigned ID, const char* filename, bool looped); /* BGM“Ç‚İ‚İ */
void soundload(unsigned ID, const char* filename, bool looped); /* ƒTƒEƒ“ƒh“Ç‚İ‚İ */
void bgmstop(); /* BGM’â~ */
void bgmplay(unsigned ID); /* ‚a‚f‚lÄ¶ */
void setvolume(); /* ‰¹—Êİ’è‚ğ”½‰f */

}
}
