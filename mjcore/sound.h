#ifndef SOUND_H
#define SOUND_H

#include "../sound/sound.h"
#include "bgmid.h"
#include "strcode.h"
#include "func.h"
#include <vector>

namespace sound {
namespace util {

void bgmload(unsigned ID, LPCSTR filename, bool looped); /* BGM“Ç‚İ‚İ */
void soundload(unsigned ID, LPCSTR filename, bool looped); /* ƒTƒEƒ“ƒh“Ç‚İ‚İ */
void bgmstop(); /* BGM’â~ */
void bgmplay(unsigned ID); /* ‚a‚f‚lÄ¶ */

}
}

#endif
