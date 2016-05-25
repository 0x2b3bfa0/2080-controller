/*
COPYRIGHT (c) 2016 Helio Machado <crushedice2000@gmail.com>

GNU GENERAL PUBLIC LICENSE
   Version 3, 29 June 2007

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Die Zauberflöte - Der Vogelfänger bin ich ja

#include "IRremote.h"
#include "IRremoteInt.h"


#define BITS       8        // The number of bits per number.

#define HDR_MARK   8300     // The header mark (the first mark)
#define SEPARATOR  4100     // The separator space precedes each byte.

#define BIT_MARK   500      // The bit mark precedes each bit.
#define ZERO_SPACE 550      // --> These spaces are the ones an the zeros
#define ONE_SPACE  1600     // --> respectively.

#define LENGTH (2 + (2 * BITS + 1) + 1 + (2 * BITS + 1) + 1)
//              ^    ^^^^^^^^^^^^    ^    ^^^^^^^^^^^^    ^
//              |         |          |         |          |__ One more because
//							|		   		|				   |				 |							the buffer count
//              |         |          |         |              is 1-indexed.
//              |         |          |         |
//              |         |          |         |__ The second byte, plus 8 bit
//              |         |          |             marks, plus the last mark.
//              |         |          |
//              |         |          |__ The field separator mark (SEPARATOR)
//              |         |              before the second byte.
//              |         |
//              |         |__ The first byte, plus 8 bit marks,
//              |             plus the last mark.
//              |
//              |__ HDR_MARK and SEPARATOR
//                  (the field separator mark before the first byte).

// Key array to use for key identification and automatic typing.
#if SEND_VELLEMAN || DECODE_VELLEMAN
	const char * const buttons[44][5] = {
		{"ON/OFF",   0,   0,   0,   0},     {"TIME/DATE",    0,   0,   0,   0},      {"Q-PROG",     0,   0,   0,   0},      {"ESC",     0,   0,   0,   0},
		{"SHIFT",    0,   0,   0,   0},     {"EUROP-CHAR",   0,   0,   0,   0},      {"PROG",       0,   0,   0,   0},      {"FUNCT",   0,   0,   0,   0},
		{"COLOR",    0,   0,   0,   0},     {"FONT",         0,   0,   0,   0},      {"RUN-PAGE",   0,   0,   0,   0},      {"ENTER",   0,   0,   0,  "\n"},
		{"LEFT",     0,   0,   0,   0},     {"RIGHT",        0,   0,   0,   0},      {"UP",         0,   0,   0,   0},      {"DOWN",    0,   0,   0,   0},
		{"A",       "a", "1", "!",  0},     {"B",           "b", "2", ";",  0},      {"C",         "c", "3", ":",  0},      {"D",      "d", "$",  0,   0},
		{"E",       "e", "4", "'",  0},     {"F",           "f", "5", "`",  0},      {"G",         "g", "6", "\"", 0},      {"H",      "h", "£",  0,   0},
		{"I",       "i", "7", "&",  0},     {"J",           "j", "8", "\\", 0},      {"K",         "k", "9", "%",  0},      {"L",      "l", "¥",  0,   0},
		{"M",       "m", "*", "?",  0},     {"N",           "n", "0", "[",  0},      {"O",         "o", "#", "]",  0},      {"P",      "p", "@",  0,   0},
		{"Q",       "q", "+",  0,   0},     {"R",           "r", "-",  0,   0},      {"S",         "s", "/",  0,   0},      {"T",      "t", "=",  0,   0},
		{"U",       "u", "(",  0,   0},     {"V",           "v", ")",  0,   0},      {"W",         "w", "<",  0,   0},      {"X",      "x", ">",  0,   0},
		{"Y",       "y", ".",  0,   0},     {"Z",           "z", ",",  0,   0},      {"SPACE",      0,   0,   0,  " "},     {"DEL",     0,   0,   0,   0}
	};
#endif

// Key defines to use with IRsend::send(uint8_t);
#define VELLEMAN_ON_OFF_KEY      0
#define VELLEMAN_TIME_DATE_KEY   1
#define VELLEMAN_Q_PROG_KEY      2
#define VELLEMAN_ESC_KEY         3
#define VELLEMAN_SHIFT_KEY       4
#define VELLEMAN_EUROP_CHAR_KEY  5
#define VELLEMAN_PROG_KEY        6
#define VELLEMAN_FUNCT_KEY       7
#define VELLEMAN_COLOR_KEY       8
#define VELLEMAN_FONT_KEY        9
#define VELLEMAN_RUN_PAGE_KEY    10
#define VELLEMAN_ENTER_KEY       11
#define VELLEMAN_LEFT_KEY        12
#define VELLEMAN_RIGHT_KEY       13
#define VELLEMAN_UP_KEY          14
#define VELLEMAN_DOWN_KEY        15
#define VELLEMAN_A_KEY           16
#define VELLEMAN_B_KEY           17
#define VELLEMAN_C_KEY           18
#define VELLEMAN_D_KEY           19
#define VELLEMAN_E_KEY           20
#define VELLEMAN_F_KEY           21
#define VELLEMAN_G_KEY           22
#define VELLEMAN_H_KEY           23
#define VELLEMAN_I_KEY           24
#define VELLEMAN_J_KEY           25
#define VELLEMAN_K_KEY           26
#define VELLEMAN_L_KEY           27
#define VELLEMAN_M_KEY           28
#define VELLEMAN_N_KEY           29
#define VELLEMAN_O_KEY           30
#define VELLEMAN_P_KEY           31
#define VELLEMAN_Q_KEY           32
#define VELLEMAN_R_KEY           33
#define VELLEMAN_S_KEY           34
#define VELLEMAN_T_KEY           35
#define VELLEMAN_U_KEY           36
#define VELLEMAN_V_KEY           37
#define VELLEMAN_W_KEY           38
#define VELLEMAN_X_KEY           39
#define VELLEMAN_Y_KEY           40
#define VELLEMAN_Z_KEY           41
#define VELLEMAN_SPACE_KEY       42
#define VELLEMAN_DEL_KEY         43
