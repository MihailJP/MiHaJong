#pragma once

/* X11 Key Code */

#ifndef _WIN32
#ifdef __MACH__
#define DIK_1            26
#define DIK_2            27
#define DIK_3            28
#define DIK_4            29
#define DIK_5            31
#define DIK_6            30
#define DIK_7            34
#define DIK_8            36
#define DIK_9            33
#define DIK_0            37
#define DIK_MINUS        35
#define DIK_CIRCUMFLEX   32
#define DIK_YEN         101
#define DIK_BACK         59
#define DIK_LEFT        131
#define DIK_H            12
#define DIK_UP          134
#define DIK_J            46
#define DIK_DOWN        133
#define DIK_K            48
#define DIK_RIGHT       132
#define DIK_L            45
#define DIK_HOME        123
#define DIK_END         127
#define DIK_PRIOR       124
#define DIK_NEXT        129
#define DIK_RETURN       44
#define DIK_SPACE        57
#define DIK_Z            14
#define DIK_ESCAPE       61
#define DIK_X            15
#define DIK_TAB          56
#else /*__MACH__*/
#define DIK_1            10
#define DIK_2            11
#define DIK_3            12
#define DIK_4            13
#define DIK_5            14
#define DIK_6            15
#define DIK_7            16
#define DIK_8            17
#define DIK_9            18
#define DIK_0            19
#define DIK_MINUS        20
#define DIK_CIRCUMFLEX   21
#define DIK_YEN         132
#define DIK_BACK         22
#define DIK_LEFT        113
#define DIK_H            43
#define DIK_UP          111
#define DIK_J            44
#define DIK_DOWN        116
#define DIK_K            45
#define DIK_RIGHT       114
#define DIK_L            46
#define DIK_HOME        110
#define DIK_END         115
#define DIK_PRIOR       112
#define DIK_NEXT        117
#define DIK_RETURN       36
#define DIK_SPACE        65
#define DIK_Z            52
#define DIK_ESCAPE        9
#define DIK_X            53
#define DIK_TAB          23
#endif /*__MACH__*/
#endif /*_WIN32*/
