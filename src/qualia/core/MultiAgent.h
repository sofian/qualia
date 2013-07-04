/*
 * MultiAgent.h
 *
 * An agent that is composed of multiple sub-agents.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
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
  /// The array of sub-agents.
  Agent** agents;

  /// The number of sub-agents.
  int nAgents;

  // TODO: not sure if this should be the case...
  /// True if this MultiAgent owns the pointers to the agents (and will thus free them at destruction).
  bool ownsAgents;

  /**
   * Constructor. Creates a multi-agent of *nAgents* sub-agents. Sub-agents can be assigned on-the-spot
   * by specifying *assign* or later on by assigning them eg. this->agents[i] = new MyAgent().
   * If such is the case (ie. if *assign* is NULL), then the MultiAgent will also own the agents.
   */
  MultiAgent(int nAgents, Agent** assign=0);

  /// Class destructor (frees the sub-agents if ownsAgents is true).
  virtual ~MultiAgent();

  // Agent methods.
  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  virtual void end(const Observation* observation);

  /**
   * Returns an observation suitable for sub-agent *agentIdx* starting from observation *observation* as
   * received by this MultiAgent.
   */
  virtual Observation* extractAgentObservation(int agentIdx, const Observation* observation) = 0;

  /**
   * Appends action from sub-agent *agentIdx* to the action that will later be returned by the call to
   * combineAgentActions().
   */
  virtual void appendAgentAction(int agentIdx, const Action* agentAction) = 0;

  /**
   * Returns the combine action that is the result of combining all the sub-agent actions through calls
   * to appendAgentAction(agentIdx, agentAction).
   */
  virtual Action* combineAgentActions() = 0;
};

#endif /* MULTIAGENT_H_ */
