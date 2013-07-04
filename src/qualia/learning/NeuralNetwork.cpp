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

NeuralNetwork::NeuralNetwork(unsigned int nInputs_,
                             unsigned int nHiddens_,
                             unsigned int nOutputs_,
                             float learningRate_,
                             float decreaseConstant_,
                             float weightDecay_,
                             bool linearOutput_)
 : learningRate(learningRate_),
   decreaseConstant(decreaseConstant_),
   weightDecay(weightDecay_)
{
  _nParams = nHiddens_ * (nInputs_ + 1) + nOutputs_ * (nHiddens_ + 1);

  // Allocate weights and deltas.
  weights  = (real*) Alloc::malloc( _nParams * sizeof(real) );
  dWeights = (real*) Alloc::malloc( _nParams * sizeof(real) );

  // Initialize to zero.
  memset(weights,  0, _nParams*sizeof(real));
  memset(dWeights, 0, _nParams*sizeof(real));

  // Allocate/assign layers.
  unsigned int k=0;
  _allocateLayer(inputLayer, 0, nInputs_, k);
  _allocateLayer(hiddenLayer, nInputs_, nHiddens_, k);
  _allocateLayer(outputLayer, nHiddens_, nOutputs_, k, linearOutput_);
  //init();
}

NeuralNetwork::~NeuralNetwork() {
  Alloc::free(weights);
  Alloc::free(dWeights);
  _deallocateLayer(inputLayer);
  _deallocateLayer(hiddenLayer);
  _deallocateLayer(outputLayer);
}

void NeuralNetwork::init() {
  // Reset learning parameters.
  _learningRateDiv = 1;

  // Randomize weights.
  for (unsigned int i=0; i<_nParams; i++) {
    weights[i] = randomUniform(-1, +1);
    dWeights[i] = 0;
  }
}

float NeuralNetwork::getCurrentLearningRate() const {
  return learningRate / _learningRateDiv;
}

void NeuralNetwork::setInput(int i, real x) {
  inputLayer.output[i] = x;
}

void NeuralNetwork::setInputs(const real *inputs) {
  memcpy(inputLayer.output, inputs, inputLayer.n*sizeof(real));
}

real NeuralNetwork::getOutput(int i) const {
  return outputLayer.output[i];
}

void NeuralNetwork::getOutputs(real *outputs) const {
  memcpy(outputs, outputLayer.output, outputLayer.n*sizeof(real));
}

void NeuralNetwork::backpropagate(real *outputError) {
  // Initialize output error.
  for (unsigned int i=0; i<outputLayer.n; i++) {
    outputLayer.error[i] = outputError[i];
    if (!outputLayer.linear) {
      real out = outputLayer.output[i];
      outputLayer.error[i] *= out * (1 - out);
    }
  }

  // Backpropagate inferior layers.
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
  // Update learning rate.
  float lr = getCurrentLearningRate();

  // Update weights.
  for (unsigned int i=0; i<_nParams; i++)
//    weights[i] -= learningRate * dWeights[i];
    weights[i] -= lr * (dWeights[i] + weightDecay * weights[i]);

  // Clear derivatives.
  clearDelta();

  _learningRateDiv += decreaseConstant;
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

void NeuralNetwork::_allocateLayer(Layer& layer, unsigned int nInputs, unsigned int nOutputs, unsigned int& k, bool isLinear) {
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
  layer.linear = isLinear;
}

void NeuralNetwork::_propagateLayer(Layer& lower, Layer& upper) {
//void NeuralNetwork::_propagateLayer(real* layerInputs, real* layerWeights, real* layerOutputs, int nLayerInputs, int nLayerOutputs) {
  unsigned int k=0;
  for (unsigned int i=0; i<upper.n; i++) {
    real sum = 0.0;
    for (unsigned int j=0; j<lower.n; j++)
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

    upper.output[i] = ( upper.linear ?
                          sum :
                          1.0f / (1.0f + exp(-sum)) );
  }
}

void NeuralNetwork::_backpropagateLayer(Layer& upper, Layer& lower) {
  for (unsigned int i=0; i<=lower.n; i++) {
    real out = (i < lower.n ? lower.output[i] : 1); // last element is bias
    real err = 0;
    // NOTE: Variable k is used to iterate through upper weights column i
    for (unsigned int j=0, k=i; j<upper.n; j++, k+=lower.n) {
      err += upper.weight[k] * upper.error[j];   // k = j*lower.n + i: corresponds to w_{ji}
      upper.dWeight[k] += out * upper.error[j];
    }
    // delta sigmoid
    if (i < lower.n) {
      if (! lower.linear) // non-linear: multiply error by delta-sigmoid
        err *= out * (1 - out);
      lower.error[i] = err;
    }
  }

}

void NeuralNetwork::_deallocateLayer(Layer& layer) {
  Alloc::free(layer.output);
  Alloc::free(layer.error);
}

void NeuralNetwork::save(XFile* file) {
  GradientFunction::save(file);
  file->write(&learningRate,     sizeof(real), 1);
  file->write(&decreaseConstant, sizeof(real), 1);
  file->write(&weightDecay,      sizeof(real), 1);
  file->write(&_learningRateDiv,  sizeof(real), 1);
}

void NeuralNetwork::load(XFile* file) {
  GradientFunction::load(file);
  file->read(&learningRate,     sizeof(real), 1);
  file->read(&decreaseConstant, sizeof(real), 1);
  file->read(&weightDecay,      sizeof(real), 1);
  file->read(&_learningRateDiv, sizeof(real), 1);
}


