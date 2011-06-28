/*
 * NeuralNetwork.cpp
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
#include "NeuralNetwork.h"
#include <stdlib.h>
#include <math.h>
#include "Random.h"

//#include "Print.h"
//#include "Math.h"

void NeuralNetwork::_allocateLayer(Layer& layer, int nInputs, int nOutputs, int& k) {
  layer.n = nOutputs;
  layer.output   = (real*) malloc( nOutputs * sizeof(real) );
  layer.error    = (real*) malloc( nOutputs * sizeof(real) );
  if (nInputs > 0) {
    layer.weight   = (real**) malloc( nOutputs * sizeof(real*) );
    layer.dWeight  = (real**) malloc( nOutputs * sizeof(real*) );
    for (int i=0; i<nOutputs; i++) {
      layer.weight[i]  = &_weights[k];
      layer.dWeight[i] = &_dWeights[k];
      k += (nInputs + 1);
    }
  }
  else {
    layer.weight  = 0; // NULL
    layer.dWeight = 0; // NULL
  }
}

//void NeuralNetwork::_deallocateLayer(Layer& layer) {
//  free(layer.output);
//  free(layer.error);
//  if (layer.weight)
//    free(layer.weight);
//  if (layer.dWeight)
//    free(layer.dWeight);
//}

void NeuralNetwork::_propagateLayer(Layer& lower, Layer& upper) {
  for (int i=0; i<upper.n; i++) {
    double sum = 0.0;
    for (int j=0; j<lower.n; j++)
      sum += upper.weight[i][j] * lower.output[j]; // TODO: pas super efficace, on devrait selectionner d'abord weight[i] et ensuite iterer sur les j
    sum += upper.weight[i][lower.n]; // bias

//#ifdef DEBUG
//    if (sum != sum) // nan
//    {
//      println("SUM is nan");
//      println(sum);
//      debug();
//    }
//    real wtf = exp(-sum);
//    if (wtf != wtf) // nan
//    {
//      println("exp(-sum) is nan");
//      println(wtf);
//      ::print("SUM: "); println(sum);
//      debug();
//    }
//#endif
    upper.output[i] = 1.0f / (1.0f + exp(-sum)); // sigmoid function
  }
}

void NeuralNetwork::_backpropagateLayer(Layer& upper, Layer& lower) {
  for (int i=0; i<=lower.n; i++) {
    real out = (i < lower.n ? lower.output[i] : 1); // last element is bias
    real err = 0;
    for (int j=0; j<upper.n; j++) {
      err += upper.weight[j][i] * upper.error[j];
      upper.dWeight[j][i] += out * upper.error[j];
    }
    // delta sigmoid
    if (i < lower.n)
      lower.error[i] = out * (1 - out) * err;
  }

}

void NeuralNetwork::_updateLayer(Layer& upper, Layer& lower) {
  for (int i=0; i<upper.n; i++) {
    real err = upper.error[i];
    for (int j=0; j<lower.n; j++) {
      real out = lower.output[j];
      real delta = upper.dWeight[i][j];
      real x = _learningRate * err * out;
      upper.weight[i][j] += x * delta;
    }
  }
}

void NeuralNetwork::_allocate(int nInputs, int nHidden, int nOutputs) {
  _nParams = nHidden * (nInputs + 1) + nOutputs * (nHidden + 1);

  _weights  = (real*) malloc( _nParams * sizeof(real) );
  _dWeights = (real*) malloc( _nParams * sizeof(real) );

  int k = 0;
  _allocateLayer(_inputLayer, 0, nInputs, k);
  _allocateLayer(_hiddenLayer, nInputs, nHidden, k);
  _allocateLayer(_outputLayer, nHidden, nOutputs, k);
}

NeuralNetwork::NeuralNetwork(int nInputs,
                              int nHidden,
                              int nOutputs,
                              float learningRate) {
  _allocate(nInputs, nHidden, nOutputs);
  _learningRate = learningRate;
  initialize();
}

//void NeuralNetwork::_deallocateLayer(Layer& layer) {
//  free(layer.output);
//  free(layer.error);
//  if (layer.weight) {
//    free(layer.weight);
//    free(layer.dWeight);
//  }
//}
//
//void NeuralNetwork::_deallocate() {
//  free(_weights);
//  free(_dWeights);
//  _deallocateLayer(_inputLayer);
//  _deallocateLayer(_hiddenLayer);
//  _deallocateLayer(_outputLayer);
//}


//NeuralNetwork::~NeuralNetwork() {
//  free(_weights);
//  free(_dWeights);
//  _deallocateLayer(_inputLayer);
//  _deallocateLayer(_hiddenLayer);
//  _deallocateLayer(_outputLayer);
////  _deallocate();
//}

void NeuralNetwork::initialize() {
  // randomize weights
  for (int i=0; i<_nParams; i++) {
    _weights[i] = Random::boundedUniform(-1, +1);
    _dWeights[i] = 0;
  }
}

void NeuralNetwork::setInput(real *input) {
  for (int i=0; i<_inputLayer.n; i++)
    _inputLayer.output[i] = input[i];
}

void NeuralNetwork::getOutput(real *output) const {
  for (int i=0; i<_outputLayer.n; i++) {
    output[i] = _outputLayer.output[i];
  }
}

void NeuralNetwork::clearDelta() {
  for (int i=0; i<_nParams; i++)
    _dWeights[i] = 0;
}

void NeuralNetwork::backpropagate(real *outputError) {
  // Initialize output error.
  for (int i=0; i<_outputLayer.n; i++) {
    real out = _outputLayer.output[i];
//    print("OUT:"); println(out);
    _outputLayer.error[i] = out * (1 - out) * outputError[i];
  }
  _backpropagateLayer(_outputLayer, _hiddenLayer);
  _backpropagateLayer(_hiddenLayer, _inputLayer);
}

void NeuralNetwork::propagate() {
  _propagateLayer(_inputLayer, _hiddenLayer);
  _propagateLayer(_hiddenLayer, _outputLayer);
}

void NeuralNetwork::update() {
  for (int i=0; i<_nParams; i++)
    _weights[i] += _learningRate * _dWeights[i];
  clearDelta();
}

void NeuralNetwork::setWeights(real* weights) {
  for (int i=0; i<_nParams; i++) {
    _weights[i] = weights[i];
    _dWeights[i] = 0;
  }
}

//void NeuralNetwork::save() {
//}
//
//void NeuralNetwork::load() {
//}

//void NeuralNetwork::loadXFile(XFile *file)
//{
//  file->taggedRead(_weights, sizeof(real), _nParams, "WEIGHTS");
//}
//
//void NeuralNetwork::saveXFile(XFile *file)
//{
//  file->taggedWrite(_weights, sizeof(real), _nParams, "WEIGHTS");
//}

#ifdef DEBUG

void NeuralNetwork::printLayer(Layer* layer, Layer* lower) {
  ::print("O:"); printArray(layer->output, layer->n);
  ::print("E:"); printArray(layer->error, layer->n);
  if (layer->weight) {
    ::print("W:"); printArray(layer->weight[0], layer->n * (lower->n + 1));
    ::print("dW:"); printArray(layer->dWeight[0], layer->n * (lower->n + 1));
  }
}

void NeuralNetwork::debug() {
  println("NEURAL-NET=======================");
  ::print("n. params:"); println(_nParams);
  println("------OUTPUTS------"); printLayer(&_outputLayer, &_hiddenLayer);
  println("------HIDDEN-------"); printLayer(&_hiddenLayer, &_inputLayer);
  println("------INPUTS-------"); printLayer(&_inputLayer, 0);
}
#endif


