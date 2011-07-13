/*
 * Qualia.cpp
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry | info(@)sofianaudry(.)com
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

  totalReward=0;
  nSteps=0;
  nEpisodes=0;
/* **WORRYSOME** */
//  return task_spec;
}

const observation_action_t* Qualia::start() {
  observation_t lastState=0;
  /*Make this static so that it is safe to return it*/
  static observation_action_t oa;

  totalReward=0;
  nSteps=1;

  lastState = environment->start();
  lastAction = agent->start(lastState);

  oa.observation = lastState;
  oa.action = lastAction;

/* **WORRYSOME** */
  return &oa;
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

const reward_observation_action_terminal_t* Qualia::step() {
  static reward_observation_action_terminal_t roa={0,0,0,0};
  const reward_observation_terminal_t *ro;
  real thisReward=0;
  observation_t lastState;

  //  __RL_CHECK_STRUCT(lastAction)
  ro = environment->step(lastAction);
  //  __RL_CHECK_STRUCT(ro->observation)
  thisReward = ro->reward;
  lastState = ro->observation;

  roa.reward = ro->reward;
  roa.observation = ro->observation;
  roa.terminal = ro->terminal;

  totalReward += thisReward;

  /* Sept 28/08, The reason that we only increment stepcount if we're not terminal is that if an episode ends on
  its first env_step, num_step will be 1 from env_start, but we don't want to go to num_step=2.*/
   if (ro->terminal == 1) {
     nEpisodes++;
     agent->end(thisReward);
   }
   else {
     nSteps++;
     lastAction = agent->step(thisReward,lastState);
     //     __RL_CHECK_STRUCT(lastAction)
     roa.action = lastAction;
   }

/* **WORRYSOME** */
   return &roa;
}

void Qualia::cleanup() {
  environment->cleanup();
  agent->cleanup();
}
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

int Qualia::episode(const unsigned int maxStepsThisEpisode) {
  const reward_observation_action_terminal_t *rlStepResult=0;
  int isTerminal=0;

  start();
  /*RL_start sets steps to 1*/
    for (; !isTerminal && (maxStepsThisEpisode == 0 ? 1 : nSteps < (int)maxStepsThisEpisode); ) {
      rlStepResult=step();
    isTerminal=rlStepResult->terminal;
    }

  /*Return the value of terminal to tell the caller whether the episode ended naturally or was cut off*/
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
