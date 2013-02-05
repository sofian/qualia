/*
 * test_simple_firmware.cpp
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

#include "SimpleFirmware.h"

#define LED_OUT 9
#define PHOTO_AIN 1

int main(int argc, char *argv[]) {
  char* serialPort = argv[1];

  if (!Arduino::init(serialPort)) {
    printf("Cannot open serial port %s.\n", serialPort);
    return -1;
  }

  for (int nid = 4; nid <= 4; nid++) {
    Arduino arduino(nid);

    printf("Init node #%d\n", nid);

    arduino.pinMode(LED_OUT, OUTPUT);
    arduino.delay(1000);
    printf("done\n");

    for (int i=0; i<3; i++) {
      int val;

      printf("LED on\n");
      arduino.digitalWrite(LED_OUT, HIGH);
      arduino.delay(1000);
      printf("done.\n");

      val = arduino.analogRead(PHOTO_AIN);
      printf("Photo value: %d (%d %%)\n", val, val * 100 / 1023);
      arduino.delay(1000);

      printf("LED off\n");
      arduino.digitalWrite(LED_OUT, LOW);
      arduino.delay(1000);
      printf("done.\n");

      val = arduino.analogRead(PHOTO_AIN);
      printf("Photo value: %d (%d %%)\n", val, val * 100 / 1023);
      arduino.delay(1000);

      printf("LED slowly on\n");
      for (int i=0; i<=255; i++) {
        arduino.analogWrite(LED_OUT, i);
        arduino.delay(10);
      }
      printf("done.\n");
    }
  }
}
