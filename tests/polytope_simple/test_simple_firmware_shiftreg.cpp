/*
 * test_simple_firmware_shiftreg.cpp
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

/*
 Polytope 3 x 3 Matrix Test Code to Interface with Qualia (Sofian) RL
 Network.

 Author: Elio Bidinost @ xmodal
*/

#include "SimpleFirmware.h"

void setup();
void loop();

#define MASTER_NODE_ID 0

/* SHIFT REGISTER CONTROL PINS */
#define CLOCKPIN  6  // SRCK TPICB595 // Blue // ST_CP 74HC595
#define LATCHPIN  5  // RCK TPICB595 // White // ST_HP 74HC595
#define DATAPIN   4  // SER IN TPICB595 // White // DS 74HC595

/* SHIFT REGISTER CONTROL CONSTANTS */
#define ALLOUTPUTSOFF 0
#define ALLOUTPUTSON 2048 - 1

void matrix(uint16_t val);
void bangShiftRegister();

int accessDiscreteIO[13] = {ALLOUTPUTSOFF, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, ALLOUTPUTSON};

Arduino arduino(MASTER_NODE_ID);

int main(int argc, char *argv[]) {
  char* serialPort = argv[1];

  if (!Arduino::init(serialPort)) {
    printf("Cannot open serial port %s.\n", serialPort);
    return -1;
  }

  setup();
  for (;;)
    loop();
}

void setup()
{
  arduino.pinMode(CLOCKPIN, OUTPUT);
  arduino.pinMode(LATCHPIN, OUTPUT);
  arduino.pinMode(DATAPIN, OUTPUT);

  printf("Turning all off\n");
  matrix(ALLOUTPUTSOFF );
  arduino.delay(2000);

  printf("Turning all on\n");
  matrix(ALLOUTPUTSON );
  arduino.delay(2000);

  printf("Turning all off\n");
  matrix(ALLOUTPUTSOFF );
  arduino.delay(2000);

  printf("Turning all on\n");
  matrix(ALLOUTPUTSON );
  arduino.delay(2000);

  printf("One at a time\n");
//    int accessDiscreteIO[13] = {ALLOUTPUTSOFF, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, ALLOUTPUTSON};
  for (int i=1; i<=9; i++) {
    printf("Access #%d\n", i);
    matrix(accessDiscreteIO[i]);
    arduino.delay(100);
  }


////  Serial.begin(57600);
//  arduino.pinMode(CLOCKPIN, OUTPUT);
//  arduino.pinMode(LATCHPIN, OUTPUT);
//  arduino.pinMode(DATAPIN, OUTPUT);
//
  /* All LEDS off - Brute Force - Zero Out All I/O Pins */
  matrix(ALLOUTPUTSOFF);
  arduino.delay(1000);

  /* Test all I/O Pins - Move through the Shift Register,
  and turn each Led on & off, one at a time in sequence. */
  for(int i = 1; i < 12; i++)
  {
    matrix(accessDiscreteIO[i]);
    arduino.delay(100);
  }

  /* Test all I/O Pins - Move through the Shift Register,
  and turn each Led on & off, one at a time in sequence in reverse. */
  for(int i = 12; i >= 0; i--)
  {
    matrix(accessDiscreteIO[i]);
    arduino.delay(100);
  }

  /* All LEDS off - Brute Force - Zero Out All I/O Pins */
  matrix(ALLOUTPUTSOFF);
  arduino.delay(1000);

  /* Test all I/O Pins - Move through the Shift Register,
  and turn on and hold each Led, one at a time in sequence. */
  for(int i = 1; i < 13; i++)
  {
    matrix(accessDiscreteIO[i] - 1);
    arduino.delay(100);
  }

  /* All LEDS off - Brute Force - Zero Out All I/O Pins */
  matrix(ALLOUTPUTSOFF);
  arduino.delay(1000);

  /* Test all I/O Pins - Move through the Shift Register,
  and turn on and hold each Led, one at a time in sequence in reverse. */
  for(int i = 13; i > 0; i--)
  {
    matrix(accessDiscreteIO[i] - 1);
    arduino.delay(100);
  }

  /* All LEDS off - Brute Force - Zero Out All I/O Pins */
  matrix(ALLOUTPUTSOFF);
  arduino.delay(1000);
}

void loop()
{
  bangShiftRegister();
}


void bangShiftRegister()
{
  /* All On and All Off for EVER and EVER... */
//  arduino.digitalWrite(LATCHPIN, LOW);
  matrix(ALLOUTPUTSON);
//  arduino.digitalWrite(LATCHPIN, HIGH);
  //delay(shiftBits[i]*i*random(0, 250));
  arduino.delay(250);

//  arduino.digitalWrite(LATCHPIN, LOW);
  matrix(ALLOUTPUTSOFF);
//  arduino.digitalWrite(LATCHPIN, HIGH);
  //delay(shiftBits[i]*i*random(0, 250));
  arduino.delay(250);

}
  /* Shift Out Data - One Bit at a time. */
void matrix(uint16_t val)
{
  uint8_t i;
  arduino.digitalWrite(LATCHPIN, LOW);

  for (i = 0; i < 11; i++)
  {
    arduino.digitalWrite(DATAPIN, !!(val & (1 << (10 - i))));

    //Serial.print(!!(val & (1 << (10 - i))));
    arduino.digitalWrite(CLOCKPIN, HIGH);
    arduino.digitalWrite(CLOCKPIN, LOW);
  }

  arduino.digitalWrite(LATCHPIN, HIGH);
//Serial.println(" ");
}

