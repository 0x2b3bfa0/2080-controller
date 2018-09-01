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

// 6 - Zamenhof - Album Esperanto.

// Dear maintainer:
//
// Once you are done trying to 'optimize' this code,
// and have realized what a terrible mistake that was,
// please increment the following counter as a warning
// to the next guy (please don't change the datatype, overflow risk):
//
// unsigned long long int total_hours_wasted_here = 104;
//
// If you feel the desperate need of modifying the code,
// feel free do it accompanied by music. You can find the
// ones I've listened at while doing this work by looking
// up the comment below the license on each file.

#include "main.h"

static uint8_t new_value = 0;
static uint8_t value = 0;

IRsend ir_tx;
IRrecv ir_rx(4);

void setup() {
  pinMode(ADC_PIN, INPUT);
  Serial.begin(115200);
  ir_rx.enableIRIn();
}

void loop() {
  decode_results results;
  if(ir_rx.decode(&results)) {
    on_code(&results);
  } else {
    new_value = VALUE(ADC_PIN); // See footer.
    if (value != new_value) on_change();
  }
}

void on_change() {
  for (int i=0; i<SAMPLES; i++) {
    if (VALUE(ADC_PIN) != new_value) return;
    delay(100);
  }

  value = new_value;

  ir_tx.send(VELLEMAN_RUN_PAGE_KEY);
  delay(750);

  // chapuza - f. - Trabajo hecho mal ̶y̶ ̶s̶i̶n̶ ̶e̶s̶m̶e̶r̶o̶
  ir_tx.print('A' + ((value > 0) ? (value - 1) : (value + 25)));
  delay(750);

  ir_tx.send(VELLEMAN_ENTER_KEY);
  delay(750);

  ir_rx.enableIRIn();
}

void on_code(decode_results *results) {
  char* name;
  unsigned char decode_type, address, value;
  decode_type = results->decode_type;
  address = results->address;
  value = results->value;
  name = results->name;
  ir_rx.resume();
  if (decode_type == VELLEMAN) {
      Serial.print("IR receiver: address ");
      Serial.print(address, DEC);
      Serial.print(": value ");
      Serial.print(value, DEC);
      Serial.print(": ");
      Serial.print(name);
      Serial.println();
  }
}

/*
@Pedro: Yes, I could use map(), but it truncates instead of averaging.

round(analogRead(ADC_PIN) / (1024/5.0))
  ^             ^               ^
  |             |               |
  |             |               |__ 1024 ADC levels / 5 different messages (the .0 is for float calculation)
  |             |__ The analog value
  |__ round the value so if is greater than 0.5 it will become the next integer.


Table of tresholds (in ADC values):

MIN     AVG    MAX
0       51     102    :0 (Z)
102     204    307    :1 (A)
307     409    512    :2 (B)
512     614    716    :3 (C)
716     818    921    :4 (D)
921     972    1023   :5 (E)


Table of tresholds (in volts, assuming a input voltage of 5.12V, that is what I've set on the stepdown):

MIN     AVG    MAX
0       0.25   0.51    :0 (Z)
0.51    1.02   1.53    :1 (A)
1.53    2.04   2.56    :2 (B)
2.56    3.07   3.58    :3 (C)
3.58    4.09   4.60    :4 (D)
4.60    4.86   5.12    :5 (E)
*/
