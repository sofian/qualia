/*
 * OscManager.h
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef OSCMANAGER_H_
#define OSCMANAGER_H_

#ifdef _WIN32
  #define _WINSOCKAPI_
  #include <windows.h>
  #include <winbase.h>
#else
  #include <unistd.h>
#endif


#include <qualia/core/common.h>
#include <string.h>
#include <stdio.h>
#include <lo/lo.h>

class OscManager {
private:
  OscManager();
  virtual ~OscManager();

public:
  static lo_address _client;
  static lo_server_thread _server;

  static void initOsc(const char* host, const char* port, const char* remotePort);

  // Helper function: repeast char c a certain number of times and puts it in dst.
  static void repeatChar(char* dst, char c, int times);

  static lo_address client() { return _client; }
  static lo_address server() { return _server; }

  static void error(int num, const char *msg, const char *path);
};

#endif /* OSCMANAGER_H_ */
