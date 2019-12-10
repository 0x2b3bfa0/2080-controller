/*
COPYRIGHT (c) 2016 Helio Machado <0x2b3bfa0>

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

// O mio babbino caro - Gianni Schicchi - Giacomo Puccini.

#include "IRremote.h"
#include "IRremoteInt.h"
#include "ir_Velleman.h"

// Huge optimization issues. Don't panic! You can even see empty else blocks!

#if SEND_VELLEMAN
	void IRsend::println(char* data, uint8_t address, int delay_time) {
		print(data, address, delay_time);
		delay(delay_time);
		print('\n', address, delay_time);
	}

	void IRsend::println(char data, uint8_t address, int delay_time) {
		print(data, address, delay_time);
		delay(delay_time);
		print('\n', address, delay_time);
	}

	void IRsend::print(char* data, uint8_t address, int delay_time) {
		for (size_t i=0; i<strlen(data); i++) {
      if (i) delay(delay_time);
			print(data[i], address, delay_time);
		}
    if (shift) {
      delay(delay_time);
      setShift(0, address, delay_time);
    }
	}

  // The following function requires so many CPU cycles.
  // Feel free to optimize it. However, it's working so.
	void IRsend::print(char data, uint8_t address, int delay_time) {
		for (size_t i = 0; i < sizeof(buttons) / sizeof(*buttons); i++) {
			for (size_t j = 0; j < sizeof(*buttons) / sizeof(**buttons); j++) {
				if (strlen(buttons[i][j]) == 1) {
					if (buttons[i][j][0] == data) {
						if (j < 4) {
              if (!setShift(j, address, delay_time)) {
                delay(delay_time);
              }
            }
						send(i);
						return;
					}
				}
			}
		}
	}

  bool IRsend::setShift(uint8_t new_shift, uint8_t address, int delay_time) {
    if (new_shift == shift) return true;
		for (bool needs_delay = false; shift != new_shift; shift++, shift %= 4) {
      if (needs_delay) delay(delay_time);
			send(VELLEMAN_SHIFT_KEY);
      needs_delay = true;
		}
		return false;
	}
#endif
