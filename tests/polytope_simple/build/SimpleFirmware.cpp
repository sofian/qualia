/*
 * SimpleFirmware.cpp
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
#include <assert.h>

int Arduino::fd = -1;

Arduino::Arduino(uint8_t nid_) : nid(nid_) {
}

bool Arduino::init(const char* serialPort) {
  if (fd == -1) {
    fd = serialInit(serialPort, BAUDRATE);
    sleep(2); // let the Arduinos setup
    flush();
    if (fd == -1)
      return false;
  }
  return true;
}

void Arduino::pinMode(uint8_t pin, uint8_t mode) {
  assert( command(nid, PIN_MODE, pin, mode) == 0 );
}

void Arduino::digitalWrite(uint8_t pin, uint8_t val) {
  assert( command(nid, DIGITAL_WRITE, pin, val) == 0 );
}

int Arduino::digitalRead(uint8_t pin) {
  return command(nid, DIGITAL_READ, pin, 0x0);
}

int Arduino::analogRead(uint8_t pin) {
  return command(nid, ANALOG_READ, pin, 0x0);
}
//  void analogReference(uint8_t mode);
void Arduino::analogWrite(uint8_t pin, int val) {
  assert( command(nid, ANALOG_WRITE, pin, val) == 0);
}

void Arduino::serialWrite(uint8_t byte) {
  write(fd, &byte, 1);
}

uint8_t Arduino::serialRead() {
  uint8_t ret;
  while (read(fd, &ret, 1) == 0);
  return ret;
}

void Arduino::delay(unsigned long millis) {
  if (millis < 1000)
    usleep(1000 * millis);
  else
    for (int i=0; i<100; i++) { // unrolled loop
      usleep(millis); usleep(millis); usleep(millis); usleep(millis); usleep(millis);
      usleep(millis); usleep(millis); usleep(millis); usleep(millis); usleep(millis);
    }
}

uint16_t Arduino::command(uint8_t nid, uint8_t cmd, uint8_t pin, uint8_t val) {
  // Send.
//  printf("Sending command\n");
  serialWrite(MESSAGE_HEADER);
  serialWrite(nid);
  serialWrite(cmd);
  serialWrite(pin);
  serialWrite(val);

  usleep(100); // let it time to reply

  // Receive.
  char rd = serialRead();
  if (rd != MESSAGE_HEADER) {
    printf("Error: wrong message header received from node %d: '%c'\n", nid, rd);
    exit(-1);
  }
  uint8_t h = serialRead();
  uint8_t l = serialRead();

  // Return received value.
  return (uint16_t) ( (h << 8) | l );
}

void Arduino::flush() {
  while (read(fd, 0, 1) != 0); // read everything
}

int Arduino::serialInit(const char* serialPort, int baud) {
  struct termios toptions;
  int fd;

  //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
  //        serialport,baud);

  fd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)  {
    perror("Arduino::serialInit: Unable to open port ");
    return -1;
  }

  if (tcgetattr(fd, &toptions) < 0) {
    perror("Arduino::serialInit: Couldn't get term attributes");
    return -1;
  }
  speed_t brate = baud; // let you override switch below if needed
  switch(baud) {
  case 4800:   brate=B4800;   break;
  case 9600:   brate=B9600;   break;
  #ifdef B14400
  case 14400:  brate=B14400;  break;
  #endif
  case 19200:  brate=B19200;  break;
  #ifdef B28800
  case 28800:  brate=B28800;  break;
  #endif
  case 38400:  brate=B38400;  break;
  case 57600:  brate=B57600;  break;
  case 115200: brate=B115200; break;
  }
  cfsetispeed(&toptions, brate);
  cfsetospeed(&toptions, brate);

  // 8N1
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  // no flow control
  toptions.c_cflag &= ~CRTSCTS;

  toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
  toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

  toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
  toptions.c_oflag &= ~OPOST; // make raw

  // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
  toptions.c_cc[VMIN]  = 0;
  toptions.c_cc[VTIME] = 20;

  if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
    perror("Arduino::serialInit: Couldn't set term attributes");
    return -1;
  }

  return fd;
}
