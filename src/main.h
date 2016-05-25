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

// Paraguay Purahei - Recuerdos de Ypacarai.

#include <Arduino.h>
#include <IRremote.h>

#define ADC_PIN A0
#define SAMPLES 5
#define VALUE(x) (round(analogRead(x) / (1024/5.0)))

void on_change();
void on_code(decode_results *results);
void setup();
void loop();
