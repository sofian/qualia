/*
 * Qualia.h
 *
 * The main controller for the Qualia library.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * Uses source code from the RL-Glue library (http://rl-glue.googlecode.com)
 * RL-Glue is licensed under an Apache License 2.0 (compatible with Gnu GPL)
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
#ifndef QUALIA_H_
#define QUALIA_H_

#include "common.h"
#include "Agent.h"
#include "Environment.h"

struct ObservationAction {
  Observation* observation;
  Action* action;
};

// TODO verifier les experimental methods (en bas) pcq sans elles les episodes/reward total sont pas calcules
// TODO remplacer terminal par isTerminal (boolean)
class Qualia {

public:
  Agent* agent;
  Environment* environment;

  Action* lastAction;
  ObservationAction observationAction;
  int nSteps, nEpisodes;

public:
  Qualia(Agent* agent, Environment* env);

  virtual void init();
  virtual ObservationAction* start();
  virtual ObservationAction* step();
//  void cleanup();
//  const char* RL_agent_message(const char* message);
//  const char* RL_env_message(const char* message);

//  real getotalReward();
//  int nSteps();
//  int nEpisodes();
  virtual int episode(unsigned int maxSteps);

  /**
    New Experimental Methods, not part of the public API
  **/
//  const action_t* RL_agent_start(const observation_t* observation);
//  const action_t* RL_agent_step(double reward, const observation_t* observation);
//  void RL_agent_end(double reward);
//  const observation_t* RL_env_start();
//  const reward_observation_terminal_t* RL_env_step(const action_t* action);};

};

#endif /* QUALIA_H_ */
