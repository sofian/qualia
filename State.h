/*
 * State.h
 *
 * (c) 2011 Sofian Audry | info(@)sofianaudry(.)com
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
#ifndef STATE_H_
#define STATE_H_

#include "general.h"

typedef unsigned int state_discrete_t;
typedef real state_continuous_t;

class State {
protected:
//  Alloc alloc;

  unsigned int nContinuousStates;
  unsigned int nDiscreteStates;

  state_continuous_t* continuousStates;
  state_discrete_t* discreteStates;
  unsigned int* discreteStateSizes;

public:
  State(unsigned int nContinuous, unsigned int nDiscrete) :
    nContinuousStates(nContinuous), nDiscreteStates(nDiscrete) {
    continuousStates = (state_continuous_t*) malloc(nContinuousStates * sizeof(state_continuous_t));
    discreteStates = (state_discrete_t*) malloc(nDiscreteStates * sizeof(state_discrete_t));
    discreteStateSizes = (unsigned int*) malloc(nDiscreteStates * sizeof(unsigned int));
  }

  ~State() {
    free(continuousStates);
    free(discreteStates);
    free(discreteStateSizes);
  }

  unsigned int getNumContinuousStates() const { return nContinuousStates; }
  unsigned int getNumDiscreteStates() const { return nDiscreteStates; }
  unsigned int getDiscreteStateSize(int i) const { return discreteStateSizes[i]; }
  void setDiscreteStateSize(int i, unsigned int size) { discreteStateSizes[i] = size; }

  state_continuous_t getContinuousState(int i) const { return continuousStates[i]; }
  const state_continuous_t* getContinuousStates() const { return continuousStates; }
  void copyContinuousStates(state_continuous_t* copy) const {
    _copyState(copy, continuousStates, true);
  }
  void setContinuousState(int i, state_continuous_t value) { continuousStates[i] = value; }
  void setContinuousStates(state_continuous_t* value) {
    _copyState(continuousStates, value, true);
  }

  state_discrete_t getDiscreteState(int i) const { return discreteStates[i]; }
  const state_discrete_t* getDiscreteStates() const { return discreteStates; }
  void copyDiscreteStates(state_discrete_t* copy) const {
    _copyState(copy, discreteStates, false);
  }

  void setDiscreteState(int i, state_discrete_t value) { discreteStates[i] = value; }
  void setDiscreteState(state_discrete_t* value) {
    _copyState(discreteStates, value, false);
  }

private:
  // Helper function.
  void _copyState(void* to, const void* from, bool continuous) const {
    unsigned int size = (continuous ?
                           nContinuousStates * sizeof(state_continuous_t) :
                           nDiscreteStates   * sizeof(state_discrete_t));
    memcpy(to, from, size);
  }

};

#endif /* STATE_H_ */
