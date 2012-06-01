/*
 * NeuralNetwork.h
 *
 * A simple feedforward neural network with one hidden layer.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 * Inspired by code by Karsten Kutza
 * http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
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
#ifndef NEURAL_NETWORK_INC
#define NEURAL_NETWORK_INC

#include <stdlib.h>

// TODO: simplifier le code

//#define DEBUG 1

//#define BIAS 1

#include "core/common.h"

// TODO: IMPORTANT l'output layer ne devrait pas etre sigmoide mais lineaire...

class NeuralNetwork {

public:
  // Configurable parameters /////

  // Learning rate. Value should be >= 0, usually in [0, 1].
  // The learning rate is used to adjust the speed of training. The higher the learning rate the faster the
  // network is trained. However, the network has a better chance of being trained to a local minimum solution.
  // A local minimum is a point at which the network stabilizes on a solution which is not the most optimal
  // global solution. In the case of reinforcement learning, the learning rate determines to what extent the newly
  // acquired information will override the old information. A factor of 0 will make the agent not learn anything,
  // while a factor of 1 would make the agent consider only the most recent information.
  // Source:
  // http://pages.cs.wisc.edu/~bolo/shipyard/neural/tort.html
  // http://en.wikipedia.org/wiki/Q-learning#Learning_rate
  float learningRate;

  //  float decreaseConstant;
  //  float weightDecay;

  // Internal use ////////////////

  // Layer structure.
  struct Layer {
    int n;          // number of units in this layer
    real *output;   // output of ith unit
    real *error;    // error term of ith unit
    real **weight;  // connection weights to ith unit
    //float **weightSave;
    real **dWeight; // last weight deltas for momentum
  };

  // Parameters.
  real *weights;  // weights
  real *dWeights; // weights derivatives
  int nParams;    // number of parameters

  // The three MLP layers (inputs -> hidden -> outputs).
  Layer inputLayer, hiddenLayer, outputLayer;

public:
  // Interface ///////////////////

  // Constructor/destructor.
  NeuralNetwork(int nInputs,
                int nHidden,
                int nOutputs,
                float learningRate = 0.01);
//                float decreaseConstant = 0,
//                float weightDecay = 0);
  virtual ~NeuralNetwork();

  // Public methods.

  void init();

  int nInput() const { return inputLayer.n; }
  int nHidden() const { return hiddenLayer.n; }
  int nOutput() const { return outputLayer.n; }

  void setInput(real *input);

  void getOutput(real *output) const;

  void clearDelta();

  void backpropagate(real *outputError);

  void propagate();

  void update();

  // Remaps a value in [-1, 1].
  // TODO: Should be made static.
  real remapValue(real x, real minVal, real maxVal) {
    return (2 * (x - minVal) / (maxVal - minVal) - 1);
  }

#ifdef DEBUG

  void printLayer(Layer* layer, Layer* lower);

  void debug();
#endif

  // Internal ("private") methods.
  void _allocateLayer(Layer& layer, int nInputs, int nOutputs, int& k);
  void _deallocateLayer(Layer& layer);

  void _propagateLayer(Layer& lower, Layer& upper);
  void _backpropagateLayer(Layer& upper, Layer& lower);
  void _updateLayer(Layer& upper, Layer& lower);

  void _allocate(int nInputs, int nHidden, int nOutputs);
  void _deallocate();
};

#endif


