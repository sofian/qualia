/*
 * SimpleFirmware.pde
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define LED_AOUT     9
#define SPEAKER_AOUT 10

#define PHOTO_AIN    0
#define MIC_AIN      1

void test();

void setup() {
  test();
}

void loop() {

}

void test() {
  for (int i=0; i<255; i++) {
    analogWrite(LED_AOUT, i);
    delay(10);
  }
}
