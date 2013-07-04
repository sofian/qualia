/*
 * IntCondition.cpp
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

#include "BehaviorTree.h"

namespace BehaviorTree {

  template<>
  BEHAVIOR_STATUS IntCondition<NoClass>::execute(void* agent)
  {
    bool status;
    switch (test)
    {
    case LESS_THAN:   status = ((*func2)() < val); break;
    case GREATER_THAN:  status = ((*func2)() > val); break;
    case LESS_OR_EQ:  status = ((*func2)() <= val); break;
    case GREATER_OR_EQ: status = ((*func2)() >= val); break;
    case EQUAL:     status = ((*func2)() == val); break;
    case NOT_EQUAL:   status = ((*func2)() != val); break;
    }

    if (status)
      return BT_SUCCESS;
    else
      return BT_FAILURE;
  }

}
