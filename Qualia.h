/*
 * Qualia.h
 *
 *  Created on: Jul 11, 2011
 *      Author: tats
 */

#ifndef QUALIA_H_
#define QUALIA_H_

#include "common.h"
#include "Agent.h"
#include "Environment.h"

// TODO verifier les experimental methods (en bas) pcq sans elles les episodes/reward total sont pas calcules
// TODO remplacer terminal par isTerminal (boolean)
class Qualia {

private:
  Agent* agent;
  Environment* environment;
  action_t lastAction;
  real totalReward;
  int nSteps, nEpisodes;

public:
  Qualia(Agent* agent, Environment* env);

  void init();
  const observation_action_t* start();
  const reward_observation_action_terminal_t* step();
  void cleanup();
//  const char* RL_agent_message(const char* message);
//  const char* RL_env_message(const char* message);

  real totalReward();
  int nSteps();
  int nEpisodes();
  int episode(unsigned int nSteps);

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
