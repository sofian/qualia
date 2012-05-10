/*
 * NeuralNetwork.cpp
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
#include "NeuralNetwork.h"
#include <stdlib.h>
#include <math.h>
#include "util/Random.h"

//#include "Print.h"
//#include "Math.h"

//#include <stdio.h>

NeuralNetwork::~NeuralNetwork() {
  _deallocate();
}

void NeuralNetwork::init() {
  // randomize weights
  for (int i=0; i<nParams; i++) {
    weights[i] = Random::boundedUniform(-1, +1);
    dWeights[i] = 0;
  }
//  learningRateDiv = 1;
}

void NeuralNetwork::setInput(real *input) {
  for (int i=0; i<inputLayer.n; i++)
    inputLayer.output[i] = input[i];
}

void NeuralNetwork::getOutput(real *output) const {
  for (int i=0; i<outputLayer.n; i++) {
    output[i] = outputLayer.output[i];
  }
}

void NeuralNetwork::clearDelta() {
  for (int i=0; i<nParams; i++)
    dWeights[i] = 0;
}

void NeuralNetwork::backpropagate(real *outputError) {
  // Initialize output error.
  for (int i=0; i<outputLayer.n; i++) {
    real out = outputLayer.output[i];
//    print("OUT:"); println(out);
    outputLayer.error[i] = out * (1 - out) * outputError[i];
  }
  _backpropagateLayer(outputLayer, hiddenLayer);
  _backpropagateLayer(hiddenLayer, inputLayer);
}

void NeuralNetwork::propagate() {
  _propagateLayer(inputLayer, hiddenLayer);
  _propagateLayer(hiddenLayer, outputLayer);
}

void NeuralNetwork::update() {
//  float lr = learningRate / learningRateDiv;
  for (int i=0; i<nParams; i++)
    weights[i] -= learningRate * dWeights[i];
//    weights[i] -= lr * dWeights[i] - weightDecay * weights[i];
  clearDelta();

//  learningRateDiv += decreaseConstant;
}

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
  ::print("n. params:"); println(nParams);
  println("------OUTPUTS------"); printLayer(&outputLayer, &hiddenLayer);
  println("------HIDDEN-------"); printLayer(&hiddenLayer, &inputLayer);
  println("------INPUTS-------"); printLayer(&inputLayer, 0);
}
#endif

void NeuralNetwork::_allocateLayer(Layer& layer, int nInputs, int nOutputs, int& k) {
//  printf("Allocating layer: %d => %d (%d)\n", nInputs, nOutputs, k);
  layer.n = nOutputs;
  layer.output   = (real*) Alloc::malloc( nOutputs * sizeof(real) );
  layer.error    = (real*) Alloc::malloc( nOutputs * sizeof(real) );
  if (nInputs > 0) {
    layer.weight   = (real**) Alloc::malloc( nOutputs * sizeof(real*) );
    layer.dWeight  = (real**) Alloc::malloc( nOutputs * sizeof(real*) );
    for (int i=0; i<nOutputs; i++) {
//      printf("%d (%d / %d)\n", i, k, nParams);
      layer.weight[i]  = &weights[k];
      layer.dWeight[i] = &dWeights[k];
      k += (nInputs + 1);
    }
  }
  else {
    layer.weight  = 0; // NULL
    layer.dWeight = 0; // NULL
  }
}

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
      real x = learningRate * err * out;
      upper.weight[i][j] += x * delta;
    }
  }
}

void NeuralNetwork::_allocate(int nInputs, int nHidden, int nOutputs) {
  nParams = nHidden * (nInputs + 1) + nOutputs * (nHidden + 1);

  weights  = (real*) malloc( nParams * sizeof(real) );
  dWeights = (real*) malloc( nParams * sizeof(real) );

  int k = 0;
  _allocateLayer(inputLayer, 0, nInputs, k);
  _allocateLayer(hiddenLayer, nInputs, nHidden, k);
  _allocateLayer(outputLayer, nHidden, nOutputs, k);
}

NeuralNetwork::NeuralNetwork(int nInputs,
                              int nHidden,
                              int nOutputs,
                              float learningRate_)
//                              float decreaseConstant_,
//                              float weightDecay_)
 : learningRate(learningRate_)
//, decreaseConstant(decreaseConstant_), weightDecay(weightDecay_)
{
  _allocate(nInputs, nHidden, nOutputs);
  init();
}

void NeuralNetwork::_deallocateLayer(Layer& layer) {
  Alloc::free(layer.output);
  Alloc::free(layer.error);
  if (layer.weight) {
    Alloc::free(layer.weight);
    Alloc::free(layer.dWeight);
  }
}

void NeuralNetwork::_deallocate() {
  Alloc::free(weights);
  Alloc::free(dWeights);
  _deallocateLayer(inputLayer);
  _deallocateLayer(hiddenLayer);
  _deallocateLayer(outputLayer);
}

