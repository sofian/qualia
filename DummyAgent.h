/*
 * DummyAgent.h
 *
 *  Created on: Jul 11, 2011
 *      Author: tats
 */

#ifndef DUMMYAGENT_H_
#define DUMMYAGENT_H_

#include "Agent.h"

class DummyAgent : public Agent {

public:
  action_t currentAction;

  virtual void init() {
    currentAction = 0;
  }

  virtual const action_t start(const observation_t observation) {
    return currentAction;
  }

  virtual const action_t step(real reward, const observation_t observation) {
    currentAction++;
    currentAction % 100; // max action
    return currentAction;
  }

};

#endif /* DUMMYAGENT_H_ */
