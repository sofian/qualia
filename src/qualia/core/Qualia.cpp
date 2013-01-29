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
  lastAction(0), nSteps(0), nEpisodes(0)
{ }

Qualia::~Qualia() {}

void Qualia::init() {
  environment->init();
  agent->init();
  nSteps=0;
  nEpisodes=0;
}

ObservationAction* Qualia::start() {
  nSteps=1;

  Observation* lastObservation = environment->start();
  lastAction = agent->start(lastObservation);

  observationAction.observation = lastObservation;
  observationAction.action = lastAction;

  return &observationAction;
}

ObservationAction* Qualia::step() {
  Observation* lastObservation = environment->step(lastAction);

  observationAction.observation = lastObservation;

  // Note: We only increment nSteps if we're not terminal because if an episode ends on
  // its first step, nSteps will be 1 from start(), but we don't want to go to nSteps=2
   if (lastObservation->terminal) {
     nEpisodes++;
     agent->end(lastObservation);
     observationAction.action = 0; // no end action
   }
   else {
     nSteps++;
     lastAction = agent->step(lastObservation);
     observationAction.action = lastAction;
   }

   return &observationAction;
}

bool Qualia::episode(const unsigned int maxSteps) {
  ObservationAction* stepResult = 0;
  bool isTerminal = false;

  start();
  // start() sets nSteps to 1
  while (!isTerminal && (maxSteps == 0 ? 1 : nSteps < (int)maxSteps)) {
    stepResult = step();
    isTerminal = stepResult->observation->terminal;
  }

  // Increment number of episodes.
  nEpisodes++;

  // Return the value of terminal to tell the caller whether the episode ended naturally or was cut off
  return isTerminal;
}

