/*
 * test_simple_firmware_4in9.cpp
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
#include <stdio.h>

// Test for the "4-in-9" set of agents
#define N_LEDS 9

#define PHOTO_AIN 0

void setLeds(const bool ledValues[]);
void getPhotoSensors(int photoValues[]);
int getPhotoSensor(int index);

#define MAIN_NODE_ID 0
#define N_PHOTO_SENSORS 4

/* SHIFT REGISTER CONTROL PINS */
#define CLOCKPIN  6  // SRCK TPICB595 // Blue // ST_CP 74HC595
#define LATCHPIN  5  // RCK TPICB595 // White // ST_HP 74HC595
#define DATAPIN   4  // SER IN TPICB595 // White // DS 74HC595

/* SHIFT REGISTER CONTROL CONSTANTS */
#define ALLOUTPUTSOFF 0
#define ALLOUTPUTSON 2048 - 1

void matrix(uint16_t val);

Arduino ledsController(0);
Arduino photoSensors[N_PHOTO_SENSORS];

int main(int argc, char *argv[]) {

  printf("Starting\n");
  char* serialPort = argv[1];

  if (!Arduino::init(serialPort)) {
    printf("Cannot open serial port %s.\n", serialPort);
    return -1;
  }

  for (int i=0; i<N_PHOTO_SENSORS; i++)
    photoSensors[i].nid = (i+1);

  printf("Setup\n");
  ledsController.pinMode(CLOCKPIN, OUTPUT);
  ledsController.pinMode(LATCHPIN, OUTPUT);
  ledsController.pinMode(DATAPIN, OUTPUT);

  printf("Turning all off\n");
  matrix( ALLOUTPUTSOFF );
  ledsController.delay(2000);

  printf("Turning all on\n");
  matrix( ALLOUTPUTSON );
  ledsController.delay(2000);

  printf("Turning all off\n");
  matrix( ALLOUTPUTSOFF );
  ledsController.delay(2000);

  printf("Turning all on\n");
  matrix( ALLOUTPUTSON );
  ledsController.delay(2000);

  int accessDiscreteIO[13] = {ALLOUTPUTSOFF, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, ALLOUTPUTSON};
  for (int i=1; i<=9; i++) {
    printf("Access #%d\n", i);
    matrix(accessDiscreteIO[i]);
    ledsController.delay(100);
  }

//  printf("One at a time\n");
//  for (int i=0; i<N_LEDS; i++) {
//    printf("LED #%d\n", i);
//    bool vals[N_LEDS];
//    for (int j=0; j<N_LEDS; j++)
//      vals[j] = (j == i);
//    setLeds(vals);
//    ledsController.delay(2000);
//  }
//  setLeds( (const bool[])
//      { 1, 0, 0,
//        0, 1, 0,
//        0, 0, 1 } );
  for (;;);
}

void setLeds(const bool ledValues[]) {
  uint16_t val = ALLOUTPUTSOFF;
  for (int i=0; i<N_LEDS; i++)
    if (ledValues[i])
      val |= (1 << i);
  matrix( val );
}

int getPhotoSensor(int index) {
  return photoSensors[index].analogRead(PHOTO_AIN);
}

void getPhotoSensors(int photoValues[]) {
  for (int i=0; i<N_PHOTO_SENSORS; i++)
    photoValues[i] = getPhotoSensor(i);
}

/* Shift Out Data - One Bit at a time. */
void matrix(uint16_t val)
{
  uint8_t i;
  ledsController.digitalWrite(LATCHPIN, LOW);

  for (i = 0; i < 11; i++)
  {
    ledsController.digitalWrite(DATAPIN, !!(val & (1 << (10 - i))));

    //Serial.print(!!(val & (1 << (10 - i))));
    ledsController.digitalWrite(CLOCKPIN, HIGH);
    ledsController.digitalWrite(CLOCKPIN, LOW);
  }
  //Serial.println(" ");
  ledsController.digitalWrite(LATCHPIN, HIGH);
}

