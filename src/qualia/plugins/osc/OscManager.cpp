/*
 * OscManager.cpp
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

#include "OscManager.h"

lo_address OscManager::_client;
lo_server_thread OscManager::_server;

OscManager::OscManager() {
  // TODO Auto-generated constructor stub

}

OscManager::~OscManager() {
  // TODO Auto-generated destructor stub
}

void OscManager::initOsc(const char* host, const char* port, const char* remotePort) {

  _client = lo_address_new(host, remotePort);

  /* start a new server on port 7770 */
  _server = lo_server_thread_new(port, OscManager::error);

  /* add method that will match any path and args */
  // lo_server_thread_add_method(server, "/bang", "", OscManager::bangHandler, 0);
  lo_server_thread_start(_server);
}

void OscManager::repeatChar(char* dst, char c, int times) {
  memset(dst, c, times);
  dst[times] = '\0';
}

void OscManager::error(int num, const char *msg, const char *path)
{
  fflush(stdout);
  Q_ERROR("liblo server error %d in path %s: %s\n", num, path, msg);
}
