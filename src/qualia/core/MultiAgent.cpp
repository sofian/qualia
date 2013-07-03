/*
 * MultiAgent.cpp
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "MultiAgent.h"
#include <string.h>

MultiAgent::MultiAgent(int nAgents_, Agent** assign_)
  : agents(assign_), nAgents(nAgents_), ownsAgents(assign_ == 0)
{
  if (ownsAgents) {
    agents = (Agent**)Alloc::malloc(nAgents*sizeof(Agent*));
    memset(agents, 0, nAgents*sizeof(Agent*)); // initialize to null
  }
//  actions = (Action**)Alloc::malloc(nAgents*sizeof(Action*));
//  observations = (Observation**)Alloc::malloc(nAgents*sizeof(Observation*));
}

MultiAgent::~MultiAgent() {
  if (ownsAgents)
    Alloc::free(agents);
//  Alloc::free(actions);
//  Alloc::free(observations);
}

void MultiAgent::init() {
  for (int i=0; i<nAgents; i++) {
    Q_ASSERT_ERROR( agents[i] );
    agents[i]->init();
  }
}

Action* MultiAgent::start(const Observation* observation) {
  for (int i=0; i<nAgents; i++) {
    Observation* agentObservation = extractAgentObservation(i, observation);
    Action* agentAction = agents[i]->start( agentObservation );
    appendAgentAction(i, agentAction);
  }
  return combineAgentActions();
}

Action* MultiAgent::step(const Observation* observation) {
  for (int i=0; i<nAgents; i++) {
    Observation* agentObservation = extractAgentObservation(i, observation);
    Action* agentAction = agents[i]->step( agentObservation );
    appendAgentAction(i, agentAction);
  }
  return combineAgentActions();
}

void MultiAgent::end(const Observation* observation) {
  for (int i=0; i<nAgents; i++) {
    Observation* agentObservation = extractAgentObservation(i, observation);
    agents[i]->end(agentObservation);
  }
}
