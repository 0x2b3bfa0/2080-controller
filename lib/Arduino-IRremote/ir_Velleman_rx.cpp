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

// L'amour est un oiseau rebelle - Carmen - Georges Bizet.

#include "IRremote.h"
#include "IRremoteInt.h"
#include "ir_Velleman.h"

#if DECODE_VELLEMAN
	unsigned int IRrecv::rx_shift = 0;

	bool IRrecv::decodeVelleman(decode_results *results) {
		uint8_t data = 0;
		uint8_t address = 0;
		int offset = 1;

		// Check the amount of received data.
		if (irparams.rawlen != LENGTH) return false;

		// Check the header.
		if (!MATCH_MARK(results->rawbuf[offset++], HDR_MARK ))  return false;
		//                              ^^^^^^^^
	  //                                  |__ This will return the current offset
		//                                      before incrementing it. Seems easy,
		//                                      but many C++ programmers don't know
		//                                      it. Don't get anger if you knew it.

		// Check the first byte spacer
		if (!MATCH_SPACE(results->rawbuf[offset++], SEPARATOR))  return false;

		// Read the address bits
		for (int i = 0;  i < BITS;  i++) {
			// Check if the bit mark is valid
			if (!MATCH_MARK(results->rawbuf[offset++], BIT_MARK)) return false;
			// See if the current bit is 0, 1 or invalid
			if (MATCH_SPACE(results->rawbuf[offset], ONE_SPACE)) {
				address |= 1 << i;
			} else if (MATCH_SPACE(results->rawbuf[offset], ZERO_SPACE)) {
				address |= 0 << i;
			} else return false;
			offset++;
		}

	  // Skip the last mark from the address and the second byte separator
	  offset+=2;

		// Read the data bits
		for (int i = 0;  i < BITS;  i++) {
			// Check if the bit mark is valid
			if (!MATCH_MARK(results->rawbuf[offset++], BIT_MARK)) return false;
			// See if the current bit is 0, 1 or invalid
			if (MATCH_SPACE(results->rawbuf[offset], ONE_SPACE)) {
				data |= 1 << i;
			} else if (MATCH_SPACE(results->rawbuf[offset], ZERO_SPACE)) {
				data |= 0 << i;
			} else return false;
			offset++;
		}

	  // Check if the value is valid.
	  if ((data > 31 && data < 52)  ||
		    (data > 55 && data < 88)  ||
				(data > 91 && data < 156) ||
				(data > 159)) return false;

		// Convert the raw keycodes into readable ones.
	  // The keycode sending system is a botched job.
	  // The IR remote µC simply sends the port data.
	  // Maybe a bad PCB layout has the main culprit.
	  // The keys are arranged from right to left and
	  // up to down on the raw keycodes. The raw ones
	  // also have jumps between 31-52, 55-88, 91-156
	  // See the solution that I'm using to fix them.
	  // So we will convert that keycode mess to sort
	  // the codes with the same criteria as the keys
	  // on the IR remote. Ordered from left to right
	  // and up to bottom, The buttons will be sorted
	  // being 0 the first button (ON/OFF) and so on.

	  // Fix the jumps between some keycodes
	  if (data > 31) data -= 20; // --> These lines will fix the jumps between
	  if (data > 55) data -= 32; // --> some keycodes by subtracting the jump
	  if (data > 91) data -= 64; // --> to `data`, so we get adjacent keycodes

	  // Convert the keycodes from RTL (right to left) to LTR (left to right).
	  data -= 2 * (data % 4) - 3;  // See below the explanation of the formula.
    //
		// As the optimized formula above is difficult to explain,
    // I'll put below a small C code that does the same thing.
		//
		// int x = (data % 4); // x is the remainder of dividing data between 4.
		// data -= x;          // Now data will become its (floored) multiple of 4.
		// data += 3 - x;      // Reverse the keycodes. Note that all is 0-indexed.

    // If the keycode is SHIFT, increment the shift counter.
		// Ugly method to reset the counter if is greater than 3.
    if (data == VELLEMAN_SHIFT_KEY) rx_shift = (rx_shift + 1) % 4;

    // Set all the collected data into the results.
		results->bits        = BITS;
		results->value       = data;
		results->address     = address;
		results->decode_type = VELLEMAN;
		if (buttons[data][rx_shift][0] == 0) {
			results->name = (char*)buttons[data][0];
		} else {
			results->name = (char*)buttons[data][rx_shift];
		}

		// That true means "decoded correctly".
		// @Pedro, maybe commenting a return is a good idea. ;-)
		return true;
	}
#endif
