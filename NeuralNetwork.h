/*
 * NeuralNetwork.h
 *
 * A simple MLP with one hidden layer.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
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

// Inspired from
// http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
// by Karsten Kutza
#include "common.h"

// TODO: IMPORTANT l'output layer ne devrait pas etre sigmoide mais lineaire...

// TODO: a enlever (Arduino comp...)
//#include "core/XFile.h"
//using namespace Torch;

class NeuralNetwork {

public:
  struct Layer {
    int n;           // number of units in this layer
    real *output;   // output of ith unit
    real *error;    // error term of ith unit
    real **weight;  // connection weights to ith unit
    //float **weightSave;
    real **dWeight; // last weight deltas for momentum
  };

  real *weights;
  real *dWeights;
  int nParams;

  float learningRate;

  Layer inputLayer, hiddenLayer, outputLayer;

  // Internal use.
  void _allocateLayer(Layer& layer, int nInputs, int nOutputs, int& k);
  void _deallocateLayer(Layer& layer);

  void _propagateLayer(Layer& lower, Layer& upper);
  void _backpropagateLayer(Layer& upper, Layer& lower);
  void _updateLayer(Layer& upper, Layer& lower);

  void _allocate(int nInputs, int nHidden, int nOutputs);
  void _deallocate();

public:
  NeuralNetwork(int nInputs,
                int nHidden,
                int nOutputs,
                float learningRate = 0.01);
  virtual ~NeuralNetwork();

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
  real remapValue(real x, real minVal, real maxVal) {
    return (2 * (x - minVal) / (maxVal - minVal) - 1);
  }

  // TODO: ceci ne devrait pas etre dans cette classe car ca ne marchera pas sur Arduino
  /// Save all the parameters to params.
//  virtual void save();
  /// Load all the parameters from params.
//  virtual void load();

#ifdef DEBUG

  void printLayer(Layer* layer, Layer* lower);

  void debug();
#endif
};

#endif


