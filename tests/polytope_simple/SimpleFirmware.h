/*
 * SimpleFirmware.h
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

#ifndef SIMPLEFIRMWARE_H_
#define SIMPLEFIRMWARE_H_

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

// Basic definitions from wiring.h
#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

// Simple firmware definitions.
#define PIN_MODE 'M'
#define DIGITAL_WRITE 'W'
#define ANALOG_WRITE 'w'
#define DIGITAL_READ 'R'
#define ANALOG_READ 'r'

#define BAUDRATE 57600L

#define MESSAGE_HEADER 'S'

class Arduino {
public:
  static int fd;
  uint8_t nid;

  Arduino(uint8_t nid = 0);

  static bool init(const char* serialPort);

  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, uint8_t val);
  int digitalRead(uint8_t pin);
  int analogRead(uint8_t pin);
//  void analogReference(uint8_t mode);
  void analogWrite(uint8_t pin, int val);

  void delay(unsigned long millis);
  static void flush();

  // Sends a command to the arduino and return the result.
  static uint16_t command(uint8_t nid, uint8_t cmd, uint8_t pin, uint8_t val);

  static void serialWrite(uint8_t byte);
  static uint8_t serialRead();

  static int serialInit(const char* serialPort, int baud);
};

#endif /* SIMPLEFIRMWARE_H_ */
