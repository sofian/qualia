/*
 * unit_test.cpp
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

#include "common.h"
#include "Action.h"

#include <assert.h>
#include <stdio.h>

const unsigned int nActions[] = {3, 2, 2};
void testActions() {
  Action test(3, nActions);
  printf("- Testing conflated\n");
  for (action_t i=0; i<test.nConflated; i++) {
    action_t conf = test.setConflated(i).conflated();
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, conf, test[0], test[1], test[2] );
    assert( conf == i );
  }
  printf("-> PASSED\n");

  printf("- Testing iteration\n");
  test.reset();
  for (action_t i=0; test.hasNext(); test.next(), i++) {
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, test.conflated(), test[0], test[1], test[2] );
    assert( i < test.nConflated );
    assert( test.conflated() == i );
  }
  printf("-> PASSED\n");
}

int main() {
  testActions();
}
