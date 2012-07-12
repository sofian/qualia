/*
 * MultiAgent.h
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

#ifndef MULTIAGENT_H_
#define MULTIAGENT_H_

#include "Agent.h"

class MultiAgent: public Agent {
public:
  Agent** agents;
  int nAgents;
  bool ownsAgents;

  MultiAgent(int nAgents, Agent** assign=0);
  virtual ~MultiAgent();

  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  virtual void end(const Observation* observation);

  virtual Observation* extractAgentObservation(int agentIdx, const Observation* observation) = 0;
  virtual void appendAgentAction(int agentIdx, const Action* agentAction) = 0;
  virtual Action* combineAgentActions() = 0;
};

#endif /* MULTIAGENT_H_ */
