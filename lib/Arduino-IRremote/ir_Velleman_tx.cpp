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

// 2 - Mistero numero kvin - Album Esperanto.

#include "IRremote.h"
#include "IRremoteInt.h"
#include "ir_Velleman.h"

#if SEND_VELLEMAN
  unsigned int IRsend::shift = 0;

  void IRsend::send(uint8_t data, uint8_t address) {

  	// Fix the jumps between some keycodes.
    if (data > 31) data += 20; // --> See the comment on
    if (data > 55) data += 32; // --> the receiving code
    if (data > 91) data += 64; // --> to know how works.

    // Convert the keycodes from LTR (left to right) to RTL (right to left).
    data -= 2 * (data % 4) - 3;  // See below the explanation of the formula.
    //
		// As the optimized formula above is difficult to explain,
    // I'll put below a small C code that does the same thing.
		//
		// int x = (data % 4); // x is the remainder of dividing data between 4.
		// data -= x;          // Now data will become its (floored) multiple of 4.
		// data += 3 - x;      // Reverse the keycodes. Note that all is 0-indexed.

    // Set IR carrier frequency to 38 KHz and enable the IR output.
  	enableIROut(38);

  	// Send the header mark.
  	mark(HDR_MARK);

  	// Send the SEPARATOR space before sending the first byte.
  	space(SEPARATOR);

  	// Note the clever trick to send the byte:
  	// The for loop iterates over a byte = 0b00000001 shifting it
  	// until overflows. Then we use the bitwise AND operator
  	// to get and send the current bit.

    // Don't simplify the code putting the bit mark out of the condifional.
    // This will create important timing errors.

  	// Address (the first byte).
  	for (uint8_t mask = 1; mask; mask <<= 1) {
  		if (address & mask) {
  			mark(BIT_MARK);
  			space(ONE_SPACE);
  		} else {
  			mark(BIT_MARK);
  			space(ZERO_SPACE);
  		}
  	}

  	// Send the last bit mark of the first byte.
  	mark(BIT_MARK);

  	// Send the SEPARATOR space before sending the second byte.
  	space(SEPARATOR);

  	// Data (the second byte).
  	for (uint8_t mask = 1; mask; mask <<= 1) {
  		if (data & mask) {
  			mark(BIT_MARK);
  			space(ONE_SPACE);
  		} else {
  			mark(BIT_MARK);
  			space(ZERO_SPACE);
  		}
  	}

  	// Send the last bit mark of the second byte.
  	mark(BIT_MARK);

  	// Turn off the led.
    space(0);
  }
#endif
