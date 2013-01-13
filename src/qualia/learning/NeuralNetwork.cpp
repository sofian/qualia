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

NeuralNetwork::NeuralNetwork(int nInputs_,
                              int nHiddens_,
                              int nOutputs_,
                              float learningRate_,
                              float decreaseConstant_,
                              float weightDecay_)
 : learningRate(learningRate_),
   decreaseConstant(decreaseConstant_),
   weightDecay(weightDecay_)
{
  _nParams = nHiddens_ * (nInputs_ + 1) + nOutputs_ * (nHiddens_ + 1);

  weights  = (real*) Alloc::malloc( _nParams * sizeof(real) );
  dWeights = (real*) Alloc::malloc( _nParams * sizeof(real) );

  int k=0;
  _allocateLayer(inputLayer, 0, nInputs_, k);
  _allocateLayer(hiddenLayer, nInputs_, nHiddens_, k);
  _allocateLayer(outputLayer, nHiddens_, nOutputs_, k);

  init();
}

NeuralNetwork::~NeuralNetwork() {
  Alloc::free(weights);
  Alloc::free(dWeights);
  _deallocateLayer(inputLayer);
  _deallocateLayer(hiddenLayer);
  _deallocateLayer(outputLayer);
}

void NeuralNetwork::init() {
  // randomize weights
  for (int i=0; i<_nParams; i++) {
    weights[i] = randomUniform(-1, +1);
    dWeights[i] = 0;
  }
  learningRateDiv = 1;
}

void NeuralNetwork::setInput(real *input) {
  memcpy(inputLayer.output, input, inputLayer.n*sizeof(real));
}

void NeuralNetwork::getOutput(real *output) const {
  memcpy(output, outputLayer.output, outputLayer.n*sizeof(real));
}

void NeuralNetwork::backpropagate(real *outputError) {
  // Initialize output error.
  for (int i=0; i<outputLayer.n; i++) {
    real out = outputLayer.output[i];
    // TODO: changer (pas de sigmoid en output)
    outputLayer.error[i] = out * (1 - out) * outputError[i];
  }
  _backpropagateLayer(outputLayer, hiddenLayer);
  _backpropagateLayer(hiddenLayer, inputLayer);
}

void NeuralNetwork::propagate() {
  _propagateLayer(inputLayer, hiddenLayer);

//  // TODO: apply sigmoids
//  for (int i=0; i<hiddenLayer.n; i++)
//    hiddenLayer.output[i] = 1.0f / (1.0f + exp(-hiddenLayer.output[i]));

  _propagateLayer(hiddenLayer, outputLayer);
}

void NeuralNetwork::update() {
  float lr = learningRate / learningRateDiv;
  for (int i=0; i<_nParams; i++)
//    weights[i] -= learningRate * dWeights[i];
    weights[i] -= lr * (dWeights[i] + weightDecay * weights[i]);
  clearDelta();
  learningRateDiv += decreaseConstant;
}

//#ifdef DEBUG
//
//void NeuralNetwork::printLayer(Layer* layer, Layer* lower) {
//  ::print("O:"); printArray(layer->output, layer->n);
//  ::print("E:"); printArray(layer->error, layer->n);
//  if (layer->weight) {
//    ::print("W:"); printArray(layer->weight[0], layer->n * (lower->n + 1));
//    ::print("dW:"); printArray(layer->dWeight[0], layer->n * (lower->n + 1));
//  }
//}
//
//void NeuralNetwork::debug() {
//  println("NEURAL-NET=======================");
//  ::print("n. params:"); println(nParams);
//  println("------OUTPUTS------"); printLayer(&outputLayer, &hiddenLayer);
//  println("------HIDDEN-------"); printLayer(&hiddenLayer, &inputLayer);
//  println("------INPUTS-------"); printLayer(&inputLayer, 0);
//}
//#endif

void NeuralNetwork::_allocateLayer(Layer& layer, int nInputs, int nOutputs, int& k) {
  layer.n = nOutputs;
  // TODO: output not needed for inputLayer
  layer.output   = (real*) Alloc::malloc( nOutputs * sizeof(real) );
  layer.error    = (real*) Alloc::malloc( nOutputs * sizeof(real) );
  if (nInputs > 0) {
    layer.weight   = &weights[k];
    layer.dWeight  = &dWeights[k];
    k += nOutputs * (nInputs + 1);
  }
  else {
    layer.weight  = 0; // NULL
    layer.dWeight = 0; // NULL
  }
}

void NeuralNetwork::_propagateLayer(Layer& lower, Layer& upper) {
//void NeuralNetwork::_propagateLayer(real* layerInputs, real* layerWeights, real* layerOutputs, int nLayerInputs, int nLayerOutputs) {
  int k=0;
  for (int i=0; i<upper.n; i++) {
    real sum = 0.0;
    for (int j=0; j<lower.n; j++)
      sum += upper.weight[k++] * lower.output[j]; // TODO: pas super efficace, on devrait selectionner d'abord weight[i] et ensuite iterer sur les j
    sum += upper.weight[k++]; // bias

    ////#ifdef DEBUG
    ////    if (sum != sum) // nan
    ////    {
    ////      println("SUM is nan");
    ////      println(sum);
    ////      debug();
    ////    }
    ////    real wtf = exp(-sum);
    ////    if (wtf != wtf) // nan
    ////    {
    ////      println("exp(-sum) is nan");
    ////      println(wtf);
    ////      ::print("SUM: "); println(sum);
    ////      debug();
    ////    }
    ////#endif

    // TODO: devrait etre fait juste aux hidden pas aux outputs
    upper.output[i] = 1.0f / (1.0f + exp(-sum));
  }
}

void NeuralNetwork::_backpropagateLayer(Layer& upper, Layer& lower) {
  for (int i=0; i<=lower.n; i++) {
    real out = (i < lower.n ? lower.output[i] : 1); // last element is bias
    real err = 0;
    // NOTE: Variable k is used to iterate through upper weights column i
    for (int j=0, k=i; j<upper.n; j++, k+=lower.n) {
      err += upper.weight[k] * upper.error[j];   // k = j*lower.n + i: corresponds to w_{ji}
      upper.dWeight[k] += out * upper.error[j];
    }
    // delta sigmoid
    if (i < lower.n)
      lower.error[i] = out * (1 - out) * err;
  }

}

void NeuralNetwork::_deallocateLayer(Layer& layer) {
  Alloc::free(layer.output);
  Alloc::free(layer.error);
}

