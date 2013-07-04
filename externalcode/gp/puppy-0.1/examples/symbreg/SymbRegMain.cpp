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
 *  \file   SymbRegMain.cpp
 *  \brief  Implementation symbolic regression main routine.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2004/02/11 18:38:12 $
 */

/*!
 *  \defgroup SymbReg Symbolic Regression Example
 *  \brief Symbolic regression (symbreg): A simple GP example with BEAGLE Puppy.
 *
 *  \par Objective
 *  Find a function of one independent variable and one dependent variable, in
 *  symbolic form, that fits a given sample of 20 \f$(x_i,y_i)\f$ data points,
 *  where the target function is the quadratic polynomial \f$x^4 + x^3 + x^2 + x\f$.
 *
 *  \par Terminal set
 *  - X (the independent variable)
 *  - Ephemeral constants randomly generated in [-1,1]
 *
 *  \par Function set
 *  - +
 *  - -
 *  - *
 *  - /     (protected division)
 *
 *  \par Fitness cases
 *  The given sample of 20 data points \f$(x_i,y_i)\f$, randomly chosen within
 *  interval [-1,1].
 *
 *  \par Fitness
 *  \f$\frac{1.}{1.+RMSE}\f$ where RMSE is the Root Mean Square Error on the
 *  fitness cases.
 *
 *  \par Stopping criteria
 *  When the evolution reaches the maximum number of generations.
 *
 *  \par Reference
 *  John R. Koza, "Genetic Programming: On the Programming of Computers by Means
 *  of Natural Selection", MIT Press, 1992, pages 162-169.
 *
 */
 
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "puppy/Puppy.hpp"
#include "SymbRegPrimits.hpp"

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
#define MUT_SWAP_DISTRIB_PROBA_DEFAULT 0.5f
#define SEED_DEFAULT 0


using namespace Puppy;

unsigned int evaluateSymbReg(std::vector<Tree>& ioPopulation,
                             Context& ioContext,
                             const std::vector<double>& inX,
                             const std::vector<double>& inF);
                             
int main(int argc, char** argv);

void usage(char** argv);


/*!
 *  \brief Program main routine.
 *  \param argc Number of arguments given on the command-line.
 *  \param argv Command-line arguments.
 *  \ingroup SymbReg
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
  std::cout << "BEAGLE Puppy symbolic regression" << std::endl;
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
  lContext.insert(new Add);
  lContext.insert(new Subtract);
  lContext.insert(new Multiply);
  lContext.insert(new Divide);
  lContext.insert(new TokenT<double>("X", 0.0));
  lContext.insert(new Ephemeral);

  // Sample equation on 20 random points in [-1.0, 1.0].
  std::cout << "Sampling equation to regress" << std::endl;
  std::vector<double> lX(20);
  std::vector<double> lF(20);
  for(unsigned int i=0; i<lX.size(); ++i) {
    lX[i] = lContext.mRandom.rollUniform(-1.0, 1.0);
    lF[i] = lX[i]*(lX[i]*(lX[i]*(lX[i]+1.0)+1.0)+1.0);
  }
  
  // Initialize population.
  std::vector<Tree> lPopulation(lPopSize);
  std::cout << "Initializing population" << std::endl;
  initializePopulation(lPopulation, lContext, lInitGrowProba, lMinInitDepth, lMaxInitDepth);
  evaluateSymbReg(lPopulation, lContext, lX, lF);
  calculateStats(lPopulation, 0);
  
  // Evolve population for the given number of generations
  std::cout << "Starting evolution" << std::endl;
  for(unsigned int i=1; i<=lNbrGen; ++i) {
    applySelectionTournament(lPopulation, lContext, lNbrPartTournament);  
    applyCrossover(lPopulation, lContext, lCrossoverProba, lCrossDistribProba, lMaxDepth);
    applyMutationStandard(lPopulation, lContext, lMutStdProba, lMutMaxRegenDepth, lMaxDepth);
    applyMutationSwap(lPopulation, lContext, lMutSwapProba, lMutSwapDistribProba);
    evaluateSymbReg(lPopulation, lContext, lX, lF);
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
 *  \param inX Independant sample values for evaluation.
 *  \param inF Dependant sample values for evaluation.
 *  \return Number of fitness evaluated.
 *  \ingroup SymbReg
 */
unsigned int evaluateSymbReg(std::vector<Tree>& ioPopulation,
                             Context& ioContext,
                             const std::vector<double>& inX,
                             const std::vector<double>& inF)
{
  assert(inX.size() == inF.size());
  unsigned int lNbrEval = 0;
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    if(ioPopulation[i].mValid) continue;
    double lQuadErr = 0.0;
    for(unsigned int j=0; j<inX.size(); ++j) {
      ioContext.mPrimitiveMap["X"]->setValue(&inX[j]);
      double lResult = 0.0;
      ioPopulation[i].interpret(&lResult, ioContext);
      double lErr = lResult - inF[j];
      lQuadErr += (lErr * lErr);
    }
    double lRMS = std::sqrt(lQuadErr / inX.size());
    ioPopulation[i].mFitness = 1. / (1. + lRMS);
    ioPopulation[i].mValid = true;
    ++lNbrEval;
  }
  return lNbrEval;
}


/*!
 *  \brief Show program usage at STDERR and exit.
 *  \param argv Command-line arguments given to the program.
 *  \ingroup SymbReg
 */
void usage(char** argv)
{
  std::cerr << "BEAGLE Puppy symbolic regression" << std::endl;
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

