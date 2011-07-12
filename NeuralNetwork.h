/*
 * NeuralNetwork.h
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
#ifndef NEURAL_NETWORK_INC
#define NEURAL_NETWORK_INC

#include <stdlib.h>

//#define DEBUG 1

//#define BIAS 1

// Inspired from
// http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
// by Karsten Kutza
#include "common.h"
//#include "rllib/Alloc.h"

// TODO: IMPORTANT l'output layer ne devrait pas etre sigmoide mais lineaire...

// TODO: a enlever (Arduino comp...)
//#include "core/XFile.h"
//using namespace Torch;

class NeuralNetwork {

protected:
//  Alloc alloc;

  struct Layer {
    int n;           // number of units in this layer
    real *output;   // output of ith unit
    real *error;    // error term of ith unit
    real **weight;  // connection weights to ith unit
    //float **weightSave;
    real **dWeight; // last weight deltas for momentum
  };

  real *_weights, *_dWeights;

  Layer _inputLayer, _hiddenLayer, _outputLayer;
  int _nParams;

  float _learningRate;
  real _error;

  void _allocateLayer(Layer& layer, int nInputs, int nOutputs, int& k);
//  void _deallocateLayer(Layer& layer);

  void _propagateLayer(Layer& lower, Layer& upper);

  void _backpropagateLayer(Layer& upper, Layer& lower);

  void _updateLayer(Layer& upper, Layer& lower);

  void _allocate(int nInputs, int nHidden, int nOutputs);

//  void _deallocateLayer(Layer& layer);
//  void _deallocate();

public:
  NeuralNetwork(int nInputs,
                int nHidden,
                int nOutputs,
                float learningRate = 0.01);
//  virtual ~NeuralNetwork();

  void initialize();

  int nInput() const { return _inputLayer.n; }
  int nHidden() const { return _hiddenLayer.n; }
  int nOutput() const { return _outputLayer.n; }

  void setInput(real *input);

  void getOutput(real *output) const;

  void clearDelta();

  void backpropagate(real *outputError);

  void propagate();

  void update();

  void setWeights(real* weights);
  int nWeights() const { return _nParams; }
  real* weights() const { return _weights; }
  real* dWeights() const { return _dWeights; }

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


