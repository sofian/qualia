/*
 * PolytopeSimpleFirmata.pde
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

#define PIN_MODE 'M'
#define DIGITAL_WRITE 'W'
#define ANALOG_WRITE 'w'
#define DIGITAL_READ 'R'
#define ANALOG_READ 'r'

#define BAUDRATE 57600L

#define MESSAGE_HEADER 'S'

#define NODE_ID 0

void setup() {
  Serial.begin(BAUDRATE);
}

void loop() {

  if (Serial.available() >= 5) {

    if (Serial.read() == MESSAGE_HEADER) {

      // Check if this message is adressed to us.
      byte nid = Serial.read();
      if (nid != NODE_ID) {
        for (int i=0; i<3; i++)
          Serial.read(); // flush
      } else {
        // Parse the message.
        char cmd = Serial.read();
        char pin = Serial.read();
        char val = Serial.read();

        int ret = 0;

        switch (cmd) {
        case PIN_MODE:
          pinMode(pin, val); break;
        case DIGITAL_WRITE:
          digitalWrite(pin, val); break;
        case ANALOG_WRITE:
          analogWrite(pin, val); break;
        case DIGITAL_READ:
          ret = digitalRead(pin); break;
        case ANALOG_READ:
          ret = analogRead(pin); break;
        default:;
        }

        // Return message.
        Serial.print(MESSAGE_HEADER, BYTE);
        Serial.print( (ret >> 8) & 0xff, BYTE);
        Serial.print( ret & 0xff, BYTE);
      }
    }
  }
}
