/*
 * main.cpp
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
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

//#include "cpp_include.h"

#include <qualia/core/common.h>

#if ! is_computer()
#error "This program only works on computer platforms. Please recompile using platform=computer option."
#endif

#include <vector>

#include <qualia/computer/CmdLine.h>
//#include <qualia/computer/DiskXFileDataSet.h>

#include <qualia/learning/MultiDataSet.h>
#include <qualia/learning/MemoryDataSet.h>
#include <qualia/computer/SubDataSet.h>

#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/TupleDataSet.h>
#include <qualia/rl/QFunction.h>
#include <qualia/rl/BatchRLTrainer.h>

#include <cstdio>
#include <cstring>
#include <ctime>

int main(int argc, char** argv) {
  char* trainFileName;
  int nHidden;
  int maxIter;
  float learningRate;
  float learningRateDecay;
  float weightDecay;
  float gamma;
  int dimObservations;
  int dimActions;
  char* stringNActions;
  char* modelFileName;
  bool dumpWeights;
  int seed;

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info("Batch RL");

  // Train mode
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("training_set_list", &trainFileName, "training set list");
  cmd.addICmdArg("dim_observations", &dimObservations, "observation dimension (without the reward)", true);
  cmd.addICmdArg("dim_actions", &dimActions, "action dimension", true);
  cmd.addSCmdArg("n_actions", &stringNActions, "number of actions as comma-separated values", true);

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-nhu", &nHidden, 5, "number of hidden units", true);

  cmd.addText("\nLearning Options:");
  cmd.addICmdOption("-iter", &maxIter, 25, "max number of iterations (episodes)");
  cmd.addRCmdOption("-lr", &learningRate, 0.01, "learning rate", true);
  cmd.addRCmdOption("-lrd", &learningRateDecay, 0.001, "learning rate decay", true);
  cmd.addRCmdOption("-wd", &weightDecay, 0, "weight decay", true);
  cmd.addSCmdOption("-save", &modelFileName, "", "the model trainFile");
  cmd.addBCmdOption("-dump-weights", &dumpWeights, false, "dump the weights to stdout after training (useful for integrating into AVR code)");

  cmd.addText("\nReinforcement Learning Options:");
  cmd.addRCmdOption("-gamma", &gamma, 0.999, "the gamma value", true);

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-seed", &seed, -1, "the random seed (-1 = based on time)");

//  cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
//  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
//  cmd.addICmdOption("-load_valid", &max_load_valid, -1, "max number of examples to load for valid");
//  cmd.addSCmdOption("-valid", &valid_file, "", "validation file, if you want it");
//  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
//  cmd.addSCmdOption("-save", &model_file, "", "the model file");
//  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  //int mode = cmd.read(argc, argv);
  cmd.read(argc, argv);
  cmd.setWorkingDirectory(".");

  // Set random seed.
  if (seed == -1)
    randomSeed(time(NULL));
  else
    randomSeed(seed);

  // Parse n actions.
  printf("Parsing actions: ");
  unsigned int nActions[100];
  char tmp[1000];
  strcpy(tmp, stringNActions);
  int k=0;
  for (int i=0; i<dimActions-1; i++, k++) {
    Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d,%s", &nActions[k], tmp) > 0, "Malformed argument <n_actions>: %s", stringNActions);
    printf("%d ", nActions[k]);
  }
  Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d", &nActions[k]), "Malformed argument <n_actions>: %s", stringNActions);
  printf("%d \n", nActions[k]);

  ActionProperties actionProperties(dimActions, nActions);

  DiskXFile trainFile(trainFileName, "r+");
  int nTrainFiles = 0;
  char trainFileNames[100][1000];
  DataSet* tupleSets[100];
  DiskXFile* trainFiles[100];
  while (trainFile.gets(trainFileNames[nTrainFiles], 1000) != NULL) {
    if (trainFile.eof())
      break;

    trainFileNames[nTrainFiles][strlen(trainFileNames[nTrainFiles])-1] = '\0'; // remove \n
    trainFiles[nTrainFiles] = Q_NEW(DiskXFile)(trainFileNames[nTrainFiles], "r+");
    tupleSets[nTrainFiles] = Q_NEW(TupleDataSet)(trainFiles[nTrainFiles], dimObservations, &actionProperties);
    tupleSets[nTrainFiles]->init(); // TODO: ceci est mauvais: on ne devrait pas avoir à penser à faire ça. dans le constructeur le init() devrait etre appele. (???)
    nTrainFiles++;
  }

  MultiDataSet multiDataSet(tupleSets, nTrainFiles);
  MemoryDataSet memDataSet(&multiDataSet);
  DataSet* data = SubDataSet::getShuffledDataSet(&memDataSet);
  NeuralNetwork net(dimObservations + actionProperties.dim(), nHidden, 1,
                    learningRate, learningRateDecay, weightDecay,
                    false);
  QFunction qFunction(&net, dimObservations, &actionProperties);

  data->init();
//  BatchRLTrainer trainer(&qFunction, data->nExamples, gamma);
  BatchRLTrainer trainer(&qFunction, data->nExamples(), gamma, 30);

  printf("--- Starting ---\n");

  trainer.train(data, maxIter);

  printf("--- Done ---\n");

  if (strcmp(modelFileName, "") != 0) {
    printf("--- Saving ---\n");
    DiskXFile modelFile(modelFileName, "w");
    net.save(&modelFile);
  }

  if (dumpWeights) {
    printf("=== BEGIN DUMP WEIGHTS ===\n");
    printf("const real WEIGHTS[] = { ");
    for (int i=0; i<net.nParams(); i++)
      printf("%f, ", net.weights[i]);
    printf("};\n");
    printf("==== END DUMP WEIGHTS ====\n");
  }

  Q_DELETE(data);
  for (int i=0; i<nTrainFiles; i++) {
    Q_DELETE(trainFiles[i]);
    Q_DELETE(tupleSets[i]);
  }
  return 0;
}

