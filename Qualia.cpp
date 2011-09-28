/*
 * Qualia.cpp
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
#include "Qualia.h"

Qualia::Qualia(Agent* theAgent, Environment* theEnv) :
  agent(theAgent), environment(theEnv),
  lastAction(0), totalReward(0), nSteps(0), nEpisodes(0)
{

}

void Qualia::init() {
  environment->init();
  agent->init();

//  const char* task_spec;
//  task_spec = env_init();
//  agent_init(task_spec);

//  RLQ totalReward=0;
  nSteps=0;
  nEpisodes=0;
/* **WORRYSOME** */
//  return task_spec;
}

ObservationAction* Qualia::start() {
//  RLQ totalReward=0;
  nSteps=1;

  Observation* lastObservation = environment->start();
  lastAction = agent->start(lastObservation);

  observationAction.observation = lastObservation;
  observationAction.action = lastAction;

/* **WORRYSOME** */
  return &observationAction;
}

//const action_t* RL_agent_start(const observation_t* observation){
//  return agent_start(observation);
//}
//const action_t* RL_agent_step(double reward, const observation_t* observation){
//  return agent_step(reward, observation);
//}
//void RL_agent_end(double reward){
//  agent_end(reward);
//}
//const observation_t* RL_env_start(){
//    const observation_t *thisObservation=0;
//
//    totalReward=0;
//    nSteps=1;
//
//    thisObservation = env_start();
//    return thisObservation;
//}
//const reward_observation_terminal_t* RL_env_step(const action_t* action){
//    const reward_observation_terminal_t *ro=0;
//      double thisReward=0;
//
//    __RL_CHECK_STRUCT(action)
//      ro = env_step(action);
//    __RL_CHECK_STRUCT(ro->observation)
//      thisReward = ro->reward;
//
//      totalReward += thisReward;
//
//     if (ro->terminal == 1) {
//       nEpisodes += 1;
//     }
//     else {
//      nSteps+=1;
//     }
//     return ro;
//  }

ObservationAction* Qualia::step() {
  //RLQ real thisReward=0;

  Observation* lastObservation = environment->step(lastAction);

  observationAction.observation = lastObservation;

// RLQ  totalReward += thisReward;

  /* Sept 28/08, The reason that we only increment stepcount if we're not terminal is that if an episode ends on
  its first env_step, num_step will be 1 from env_start, but we don't want to go to num_step=2.*/
   if (lastObservation->terminal) {
     nEpisodes++;
     agent->end(lastObservation);
     observationAction.action = 0; // ???
   }
   else {
     nSteps++;
     lastAction = agent->step(lastObservation);
     //     __RL_CHECK_STRUCT(lastAction)
     observationAction.action = lastAction;
   }

/* **WORRYSOME** */
   return &observationAction;
}

//void Qualia::cleanup() {
//  environment->cleanup();
//  agent->cleanup();
//}
//
//const char* RL_agent_message(const char* message) {
//  const char *theAgentResponse=0;
//  const char *messageToSend=0;
//
//  if(message!=0){
//    messageToSend=message;
//  }else{
//    messageToSend="";
//  }
//
//  theAgentResponse=agent_message(messageToSend);
//  if(theAgentResponse==0){
//    return "";
//  }
//
//  return theAgentResponse;
//}
//
//const char* RL_env_message(const char* message) {
//  const char *theEnvResponse=0;
//  const char *messageToSend=0;
//
//  if(message!=0){
//    messageToSend=message;
//  }else{
//    messageToSend="";
//  }
//
//  theEnvResponse=env_message(messageToSend);
//  if(theEnvResponse==0){
//    return "";
//  }
//
//  return theEnvResponse;
//}

int Qualia::episode(const unsigned int maxSteps) {
  ObservationAction* stepResult = 0;
  bool isTerminal = false;

  start();
  // start() sets steps to 1
  while (!isTerminal && (maxSteps == 0 ? 1 : nSteps < (int)maxSteps)) {
    stepResult = step();
    isTerminal = stepResult->observation->terminal;
  }

  // Return the value of terminal to tell the caller whether the episode ended naturally or was cut off
  return isTerminal;
}

//double RL_return() {
//  return totalReward;
//}
//
//int RL_nSteps() {
//  /* number of steps of the current or just completed episodes of run */
//  return nSteps;
//}
//
//int RL_nEpisodes() {
//  return nEpisodes;
//}
