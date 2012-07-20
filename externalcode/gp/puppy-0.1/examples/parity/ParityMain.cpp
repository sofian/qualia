/*
 *  BEAGLE Puppy
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   ParityMain.cpp
 *  \brief  Implementation parity main routine.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3 $
 *  $Date: 2004/02/11 18:38:12 $
 */

/*!
 *  \defgroup Parity Even-6 Parity Example
 *  \brief Even 6-parity (parity): Boolean GP example with BEAGLE Puppy.
 *
 *  \par Objective
 *  Find a program that produces the value of the Boolean even 6-parity given
 *  six independent Boolean inputs.
 *
 *  \par Terminal set
 *  - IN0, IN1, IN2, IN3, IN4, and IN5, the inputs of the function.
 *  - 0 and 1, two Boolean constants.
 *
 *  \par Function set
 *  - AND
 *  - OR
 *  - NOT
 *  - XOR    (to make the problem harder, don't use this one)
 *
 *  \par Fitness cases
 *  All the \f$2^6 = 64\f$ combinations of the six input Booleans.
 *
 *  \par Fitness
 *  Rate of correct outputs obtained over all the 64 fitness cases.
 *
 *  \par Stopping criteria
 *  When the evolution reaches the maximum number of generations.
 *
 *  \par Reference
 *  John R. Koza, "Genetic Programming II: Automatic Discovery of Reusable
 *  Programs", MIT Press, 1994, pages 157-199.
 *
 */

 
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
 
#include "puppy/Puppy.hpp"
#include "ParityPrimits.hpp"

#define POP_SIZE_DEFAULT 100
#define NBR_GEN_DEFAULT 50
#define NBR_PART_TOURNAMENT_DEFAULT 2
#define MAX_DEPTH_DEFAULT 17
#define MIN_INIT_DEPTH_DEFAULT 2
#define MAX_INIT_DEPTH_DEFAULT 5
#define INIT_GROW_PROBA_DEFAULT 0.5f
#define CROSSOVER_PROBA_DEFAULT 0.9f
#define CROSSOVER_DISTRIB_PROBA_DEFAULT 0.9f
#define MUT_STD_PROBA_DEFAULT 0.05f
#define MUT_MAX_REGEN_DEPTH_DEFAULT 5
#define MUT_SWAP_PROBA_DEFAULT 0.05f
#define MUT_SWAP_DISTRIB_PROBA_DEFAULT 0.5
#define SEED_DEFAULT 0

using namespace Puppy;


unsigned int evaluateParity(std::vector<Tree>& ioPopulation,
                            Context& ioContext,
                            const std::vector<bool>& inTruthTable);
                             
int main(int argc, char** argv);

void usage(char** argv);


/*!
 *  \brief Program main routine.
 *  \param argc Number of arguments given on the command-line.
 *  \param argv Command-line arguments.
 *  \ingroup Parity
 */
int main(int argc, char** argv) {

  // Create parameter variables with default values.
  unsigned int  lPopSize             = POP_SIZE_DEFAULT;
  unsigned int  lNbrGen              = NBR_GEN_DEFAULT;
  unsigned int  lNbrPartTournament   = NBR_PART_TOURNAMENT_DEFAULT;
  unsigned int  lMaxDepth            = MAX_DEPTH_DEFAULT;
  unsigned int  lMinInitDepth        = MIN_INIT_DEPTH_DEFAULT;
  unsigned int  lMaxInitDepth        = MAX_INIT_DEPTH_DEFAULT;
  float         lInitGrowProba       = INIT_GROW_PROBA_DEFAULT;
  float         lCrossoverProba      = CROSSOVER_PROBA_DEFAULT;
  float         lCrossDistribProba   = CROSSOVER_DISTRIB_PROBA_DEFAULT;
  float         lMutStdProba         = MUT_STD_PROBA_DEFAULT;
  unsigned int  lMutMaxRegenDepth    = MUT_MAX_REGEN_DEPTH_DEFAULT;
  float         lMutSwapProba        = MUT_SWAP_PROBA_DEFAULT;
  float         lMutSwapDistribProba = MUT_SWAP_DISTRIB_PROBA_DEFAULT;
  unsigned long lSeed                = SEED_DEFAULT;

  // Parse command-line to set new parameter values.
  for(int i=1; i<argc; ++i) {
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
        case 'p': lPopSize = std::atoi(argv[i]+2); break;
        case 'g': lNbrGen = std::atoi(argv[i]+2); break;
        case 't': lNbrPartTournament = std::atoi(argv[i]+2); break;
        case 'd': lMaxDepth = std::atoi(argv[i]+2); break;
        case 'i': lMinInitDepth = std::atoi(argv[i]+2); break;
        case 'j': lMaxInitDepth = std::atoi(argv[i]+2); break;
        case 'h': lInitGrowProba = std::atof(argv[i]+2); break;
        case 'c': lCrossoverProba = std::atof(argv[i]+2); break;
        case 'e': lCrossDistribProba = std::atof(argv[i]+2); break;
        case 'm': lMutStdProba = std::atof(argv[i]+2); break;
        case 'n': lMutMaxRegenDepth = std::atoi(argv[i]+2); break;
        case 's': lMutSwapProba = std::atof(argv[i]+2); break;
        case 'w': lMutSwapDistribProba = std::atof(argv[i]+2); break;
        case 'r': lSeed = std::atoi(argv[i]+2); break;
        default: usage(argv); break;
      }
    }
  }

  // Display message with parameters used.
  std::cout << "BEAGLE Puppy even-6 parity" << std::endl;
  std::cout << "Copyright 2001-2004 by Christian Gagne and Marc Parizeau" << std::endl;
  std::cout << "Parameters used are:" << std::endl;
  std::cout << " Population size:                           " << lPopSize << std::endl;
  std::cout << " Number of generations:                     " << lNbrGen << std::endl;
  std::cout << " Number participants tournament:            " << lNbrPartTournament << std::endl;
  std::cout << " Maximum tree depth:                        " << lMaxDepth << std::endl;
  std::cout << " Minimum tree initialization depth:         " << lMinInitDepth << std::endl;
  std::cout << " Maximum tree initialization depth:         " << lMaxInitDepth << std::endl;
  std::cout << " Grow-type initialization proba.:           " << lInitGrowProba << std::endl;
  std::cout << " Crossover probability:                     " << lCrossoverProba << std::endl;
  std::cout << " Crossover distribution probability:        " << lCrossDistribProba << std::endl;
  std::cout << " Standard (Koza's) mutation proba.:         " << lMutStdProba << std::endl;
  std::cout << " Standard mutation max. regeneration depth: " << lMutMaxRegenDepth << std::endl;
  std::cout << " Swap point mutation probability:           " << lMutSwapProba << std::endl;
  std::cout << " Swap point mutation distribution proba.:   " << lMutSwapDistribProba << std::endl;
  std::cout << " Random number generator seed value:        " << lSeed << std::endl;
  
  // Create evolution context add primitives used into it.
  std::cout << "Creating evolution context" << std::endl;
  Context lContext;
  lContext.mRandom.seed(lSeed);
  lContext.insert(new And);
  lContext.insert(new Or);
  lContext.insert(new Xor);
  lContext.insert(new Not);
  lContext.insert(new TokenT<bool>("IN1"));
  lContext.insert(new TokenT<bool>("IN2"));
  lContext.insert(new TokenT<bool>("IN3"));
  lContext.insert(new TokenT<bool>("IN4"));
  lContext.insert(new TokenT<bool>("IN5"));
  lContext.insert(new TokenT<bool>("IN6"));
  lContext.insert(new TokenT<bool>("0", false));
  lContext.insert(new TokenT<bool>("1", true));

  // Generating truth table.
  std::cout << "Generating truth table" << std::endl;
  std::vector<bool> lTruthTable(64);
  for(unsigned int i=0; i<lTruthTable.size(); ++i) {
    unsigned int lNbOnes = (i%2);
    lNbOnes += (((i % 4) > 1) ? 1 : 0);
    lNbOnes += (((i % 8) > 3) ? 1 : 0);
    lNbOnes += (((i % 16) > 7) ? 1 : 0);
    lNbOnes += (((i % 32) > 15) ? 1 : 0);
    lNbOnes += ((i > 31) ? 1 : 0); 
    lTruthTable[i] = ((lNbOnes % 2) == 1);
  }
  
  // Initialize population.
  std::vector<Tree> lPopulation(lPopSize);
  std::cout << "Initializing population" << std::endl;
  initializePopulation(lPopulation, lContext, lInitGrowProba, lMinInitDepth, lMaxInitDepth);
  evaluateParity(lPopulation, lContext, lTruthTable);
  calculateStats(lPopulation, 0);

  // Evolve population for the given number of generations
  std::cout << "Starting evolution" << std::endl;
  for(unsigned int i=1; i<=lNbrGen; ++i) {
    applySelectionTournament(lPopulation, lContext, lNbrPartTournament);
    applyCrossover(lPopulation, lContext, lCrossoverProba, lCrossDistribProba, lMaxDepth);
    applyMutationStandard(lPopulation, lContext, lMutStdProba, lMutMaxRegenDepth, lMaxDepth);
    applyMutationSwap(lPopulation, lContext, lMutSwapProba, lMutSwapDistribProba);
    evaluateParity(lPopulation, lContext, lTruthTable);
    calculateStats(lPopulation, i);
  }
  std::cout << "End of evolution" << std::endl;

  // Outputting best individual
  std::vector<Tree>::const_iterator lBestIndividual =
    std::max_element(lPopulation.begin(), lPopulation.end());
  std::cout << "Best individual at generation " << lNbrGen << " is: ";
  std::cout << *lBestIndividual << std:: endl;

  std::cout << "Exiting program" << std::endl << std::flush;
  return 0;
}


/*!
 *  \brief Evaluate fitness of a population
 *  \param ioPopulation Population to evaluate fitness.
 *  \param ioContext Evolutionary context.
 *  \param inTruthTable Truth table of desired values.
 *  \return Number of fitness evaluated.
 *  \ingroup Parity
 */
unsigned int evaluateParity(std::vector<Tree>& ioPopulation,
                            Context& ioContext,
                            const std::vector<bool>& inTruthTable)
{
  assert(inTruthTable.size() == 64);
  unsigned int lNbrEval = 0;
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    if(ioPopulation[i].mValid) continue;
    unsigned int lGood = 0;
    for(unsigned int j=0; j<inTruthTable.size(); ++j) {
      bool lInput = ((j % 2) != 0);
      ioContext.mPrimitiveMap["IN1"]->setValue(&lInput);
      lInput = ((j % 4) > 1);
      ioContext.mPrimitiveMap["IN2"]->setValue(&lInput);
      lInput = ((j % 8) > 3);
      ioContext.mPrimitiveMap["IN3"]->setValue(&lInput);
      lInput = ((j % 16) > 7);
      ioContext.mPrimitiveMap["IN4"]->setValue(&lInput);
      lInput = ((j % 32) > 15);
      ioContext.mPrimitiveMap["IN5"]->setValue(&lInput);
      lInput = (j > 31);
      ioContext.mPrimitiveMap["IN6"]->setValue(&lInput);
      bool lResult;
      ioPopulation[i].interpret(&lResult, ioContext);
      if(lResult == inTruthTable[j]) ++lGood;
    }
    ioPopulation[i].mFitness = float(lGood) / float(inTruthTable.size());
    ioPopulation[i].mValid = true;
    ++lNbrEval;
  }
  return lNbrEval;
}


/*!
 *  \brief Show program usage at STDERR and exit.
 *  \param argv Command-line arguments given to the program.
 *  \ingroup Parity
 */
void usage(char** argv)
{
  std::cerr << "BEAGLE Puppy even-6 parity" << std::endl;
  std::cerr << "Copyright 2001-2004 by Christian Gagne and Marc Parizeau" << std::endl;
  std::cerr << "usage> " << argv[0] << " [OPTIONS]" << std::endl;
  std::cerr << "Options are:" << std::endl;
  std::cerr << " -p#: population size (def: " << POP_SIZE_DEFAULT << ")" << std::endl;
  std::cerr << " -g#: number of generations (def: " << NBR_GEN_DEFAULT << ")" << std::endl;
  std::cerr << " -t#: selection tournament size (def: "
            << NBR_PART_TOURNAMENT_DEFAULT << ")" << std::endl;
  std::cerr << " -d#: maximum tree depth (def: " << MAX_DEPTH_DEFAULT << ")" << std::endl;
  std::cerr << " -i#: min initialization tree depth (def: "
            << MIN_INIT_DEPTH_DEFAULT << ")" << std::endl;
  std::cerr << " -j#: max initialization tree depth (def: "
            << MAX_INIT_DEPTH_DEFAULT << ")" << std::endl;
  std::cerr << " -h#.#: grow initialization proba. (def: "
            << INIT_GROW_PROBA_DEFAULT << ")" << std::endl;
  std::cerr << " -c#.#: crossover probability (def: "
            << CROSSOVER_PROBA_DEFAULT << ")" << std::endl;  
  std::cerr << " -e#.#: crossover distribution probability (def: "
            << CROSSOVER_DISTRIB_PROBA_DEFAULT << ")" << std::endl;
  std::cerr << " -m#.#: standard (Koza) mutation probability (def: "
            << MUT_STD_PROBA_DEFAULT << ")" << std::endl;
  std::cerr << " -n#: standard mutation max. regeneration depth (def: "
            << MUT_MAX_REGEN_DEPTH_DEFAULT << ")" << std::endl;
  std::cerr << " -s#.#: swap point mutation probability (def: "
            << MUT_SWAP_PROBA_DEFAULT << ")" << std::endl;            
  std::cerr << " -s#.#: swap point mutation distribution probability (def: "
            << MUT_SWAP_DISTRIB_PROBA_DEFAULT << ")" << std::endl;
  std::cerr << " -r#.#: random number generator seed (def: "
            << SEED_DEFAULT << ")" << std::endl << std::endl;
  std::exit(1);
}

