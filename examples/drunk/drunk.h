#ifndef DRUNK_H_
#define DRUNK_H_

#include <qualia/core/common.h>
#include <qualia/core/Agent.h>
#include <qualia/core/Environment.h>
#include <qualia/core/Qualia.h>

const unsigned int N_ACTIONS[] = { 2 };

class DrunkAgent: public Agent {
public:
  ActionProperties props; // the action properties
  Action action; // the action that will be returned by the agent's start() and step() methods

  DrunkAgent() :
      props(1, N_ACTIONS), action(&props) {
  }
  virtual ~DrunkAgent() {
  }

  virtual void init() {
    Q_MESSAGE("Initializing agent");
  }

  virtual Action* start(const Observation* observation) {
    Q_MESSAGE("Starting agent");
    return step(observation);
  }

  virtual Action* step(const Observation* observation) {
    action[0] = random(2); // picks either 0 or 1, randomly
    Q_MESSAGE("Current observation: %f / Next action: %d.",
        observation->observations[0], action[0]);
    return &action;
  }

};

class DrunkEnvironment: public Environment {
public:
  Observation observation;

  DrunkEnvironment() :
      observation(1) {
  }
  virtual ~DrunkEnvironment() {
  }

  virtual void init() {
    Q_MESSAGE("Initializing enviroment");
  }

  virtual Observation* start() {
    observation[0] = 50; // we start in the middle
    return &observation;
  }

  virtual Observation* step(const Action* action) {
    // step left or right depending on the action
    if (action->actions[0] == 0)
      observation[0]--;
    else
      observation[0]++;
    // constrain within grid
    observation[0] = constrain(observation[0], 0.0f, 100.0f);
    return &observation;
  }
};

void runDrunk();

#endif
