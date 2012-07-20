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
 *  \file   src/Puppy.cpp
 *  \brief  Implementation of BEAGLE Puppy global functions.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4 $
 *  $Date: 2004/02/11 19:49:23 $
 */

#include <cmath>
#include <algorithm>

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/PrimitiveInline.hpp"
#include "puppy/Randomizer.hpp"
#include "puppy/Context.hpp"
#include "puppy/Tree.hpp"
#include "puppy/TokenT.hpp"
#include "puppy/Puppy.hpp"
 

/*!
 *  \brief Calculate statistics for the actual population and display results to output stream.
 *  \param inPopulation Population to compute stats on.
 *  \param inGeneration Actual generation number.
 *  \param ioOS C++ output stream to write to. std::cout by default.
 *  \ingroup Puppy
 */
void Puppy::calculateStats(const std::vector<Puppy::Tree>& inPopulation,
                           unsigned int inGeneration,
                           std::ostream& ioOS)
{
  double lAvg = 0.0;
  double lStd = 0.0;
  double lMax = 0.0;
  double lMin = 0.0;

  if(inPopulation.size() == 1) {
    assert(inPopulation.front().mValid);
    lAvg = lMax = lMin = inPopulation.front().mFitness;
  }
  else if(inPopulation.size() > 1) {
    assert(inPopulation[0].mValid);
    double lSum = inPopulation[0].mFitness;
    double lPow2Sum = (lSum * lSum);
    lMax = lMin = lSum;
    for(unsigned int i=1; i<inPopulation.size(); ++i) {
      assert(inPopulation[i].mValid);
      double lFitness = inPopulation[i].mFitness;
      lSum     += lFitness;
      lPow2Sum += (lFitness * lFitness);
      if(lFitness > lMax) lMax = lFitness;
      if(lFitness < lMin) lMin = lFitness;
    }
    lAvg = lSum / inPopulation.size();
    lStd = (lPow2Sum - ((lSum * lSum) / inPopulation.size())) / (inPopulation.size() - 1);
    lStd = std::sqrt(lStd);
  }

  ioOS << "Gen: " << inGeneration << ", avg: " << lAvg << ", std: " << lStd;
  ioOS << ", max: " << lMax << ", min: " << lMin << std::endl << std::flush;
}


/*!
 *  \brief Apply sub-tree crossover operation on a population of GP trees.
 *  \param ioPopulation Population to apply crossover on.
 *  \param ioContext Evolutionary context.
 *  \param inMatingProba Probability for each individual to be modified by crossover.
 *  \param inDistribProba Probability that a crossover exchange two sub-trees of non-terminal roots.
 *  \param inMaxTreeDepth Maximum tree depth allowed.
 *  \ingroup Puppy
 */
void Puppy::applyCrossover(std::vector<Tree>& ioPopulation,
                           Puppy::Context& ioContext,
                           float inMatingProba,
                           float inDistribProba,
                           unsigned int inMaxTreeDepth)
{
  std::vector<unsigned int> lMateVector;
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    if(ioContext.mRandom.rollUniform() <= inMatingProba) {
      lMateVector.push_back(i);
    }
  }
  std::random_shuffle(lMateVector.begin(), lMateVector.end(), ioContext.mRandom);
  if((lMateVector.size() % 2) != 0) lMateVector.pop_back();

  //for(unsigned int j=0; j<lMateVector.size(); ++j) {
  //  std::cout << j << ": " << ioPopulation[lMateVector[j]] << std::endl;
  //}

  for(unsigned int j=0; j<lMateVector.size(); j+=2) {
    mateTrees(ioPopulation[lMateVector[j]],
              ioPopulation[lMateVector[j+1]],
              ioContext,
              inDistribProba,
              inMaxTreeDepth);
  }
}


/*!
 *  \brief Exchange two sub-trees.
 *  \param ioTree1  Tree containing the first sub-tree to exchange.
 *  \param inNode1  Index of root node to sub-tree to swap in first tree.
 *  \param inStack1 Stack containing the parents to the first sub-tree root node.
 *  \param ioTree2  Tree containing the second sub-tree to exchange.
 *  \param inNode2  Index of root node to sub-tree to swap in second tree.
 *  \param inStack2 Stack containing the parents to the second sub-tree root node.
 *  \ingroup Puppy
 */
void Puppy::exchangeSubTrees(Puppy::Tree& ioTree1,
                             unsigned int inNode1,
                             const std::vector<unsigned int>& inStack1,
                             Puppy::Tree& ioTree2,
                             unsigned int inNode2,
                             const std::vector<unsigned int>& inStack2)
{
  assert(&ioTree1 != &ioTree2);
  assert(inStack1.size() > 0);
  assert(inStack2.size() > 0);
  unsigned int lSwapSize1 = ioTree1[inNode1].mSubTreeSize;
  unsigned int lSwapSize2 = ioTree2[inNode2].mSubTreeSize;
  if(lSwapSize1 <= lSwapSize2) {
    std::swap_ranges<Tree::iterator,Tree::iterator>(
      ioTree1.begin()+inNode1,
      ioTree1.begin()+inNode1+lSwapSize1,
      ioTree2.begin()+inNode2);
    ioTree1.insert(ioTree1.begin()+inNode1+lSwapSize1,
                   ioTree2.begin()+inNode2+lSwapSize1,
                   ioTree2.begin()+inNode2+lSwapSize2);
    ioTree2.erase(ioTree2.begin()+inNode2+lSwapSize1,
                  ioTree2.begin()+inNode2+lSwapSize2);
  }
  else {
    std::swap_ranges<Tree::iterator,Tree::iterator>(
      ioTree1.begin()+inNode1,
      ioTree1.begin()+inNode1+lSwapSize2,
      ioTree2.begin()+inNode2);
    ioTree2.insert(ioTree2.begin()+inNode2+lSwapSize2,
                   ioTree1.begin()+inNode1+lSwapSize2,
                   ioTree1.begin()+inNode1+lSwapSize1);
    ioTree1.erase(ioTree1.begin()+inNode1+lSwapSize2,
                  ioTree1.begin()+inNode1+lSwapSize1);
  }
  int lDiffSize = lSwapSize1 - lSwapSize2;
  for(unsigned int i=0; i<(inStack1.size()-1); ++i)
    ioTree1[inStack1[i]].mSubTreeSize -= lDiffSize;
  for(unsigned int j=0; j<(inStack2.size()-1); ++j)
    ioTree2[inStack2[j]].mSubTreeSize += lDiffSize;
}


/*!
 *  \brief Mate two GP trees for crossover.
 *  \param ioTree1 First tree to mate.
 *  \param ioTree2 Second tree to mate.
 *  \param ioContext Evolutionary context.
 *  \param inDistribProba Distribution probability.
 *  \param inMaxTreeDepth Maximum tree depth allowed.
 *  \ingroup Puppy
 */
void Puppy::mateTrees(Puppy::Tree& ioTree1,
                      Puppy::Tree& ioTree2,
                      Puppy::Context& ioContext,
                      float inDistribProba,
                      unsigned int inMaxTreeDepth)
{
  // Initial parameters checks
  assert(ioTree1.size() > 0);
  assert(ioTree2.size() > 0);

  // Crossover loop. Try the given number of attempts to mate two individuals.
  for(unsigned int i=0; i<7; ++i) {

    // Choose a type of node (branch or leaf) following the distribution probability and change the
    // node for another node of the same tree if the types mismatch.
    bool lNode1IsTerminal = true;
    if(ioTree1.size() > 1) lNode1IsTerminal = (ioContext.mRandom.rollUniform() >= inDistribProba);
    unsigned int lChoosenNode1 = ioContext.mRandom((unsigned long)ioTree1.size());
    while((ioTree1[lChoosenNode1].mPrimitive->getNumberArguments() == 0) != lNode1IsTerminal) {
      lChoosenNode1 = ioContext.mRandom((unsigned long)ioTree1.size());
    }

    bool lNode2IsTerminal = true;
    if(ioTree2.size() > 1) lNode2IsTerminal = (ioContext.mRandom.rollUniform() >= inDistribProba);
    unsigned int lChoosenNode2 = ioContext.mRandom((unsigned long)ioTree2.size());
    while((ioTree2[lChoosenNode2].mPrimitive->getNumberArguments() == 0) != lNode2IsTerminal) {
      lChoosenNode2 = ioContext.mRandom((unsigned long)ioTree2.size());
    }

    // Set first stack to the node of the first tree.
    // Check if depth is ok. Do a new crossover attempt if not.
    std::vector<unsigned int> lStack1;
    ioTree1.setStackToNode(lChoosenNode1, lStack1);
    unsigned int lNewDepthTree1 =
      lStack1.size() + ioTree2.getDepth(lChoosenNode2) - 1;
    if(lNewDepthTree1 > inMaxTreeDepth) continue;

    // Set second stack to the node of the second tree.
    // Check if depth is ok. Do a new crossover attempt if not.
    std::vector<unsigned int> lStack2;
    ioTree2.setStackToNode(lChoosenNode2, lStack2);
    unsigned int lNewDepthTree2 =
      lStack2.size() + ioTree1.getDepth(lChoosenNode1) - 1;
    if(lNewDepthTree2 > inMaxTreeDepth) continue;

    // The crossover is valid.
    exchangeSubTrees(ioTree1, lChoosenNode1, lStack1, ioTree2, lChoosenNode2, lStack2);
    ioTree1.mValid = false;
    ioTree2.mValid = false;
    break;
  }
}



/*!
 *  \brief Initialize ramped half-and-half a population of GP trees.
 *  \param ioPopulation Population to initialize.
 *  \param ioContext Evolutionary context.
 *  \param inInitGrowProba Probability to use grow initialization, in opposition to full.
 *  \param inMinDepth Minimum initialization tree depth allowed.
 *  \param inMaxDepth Maximum initialization tree depth allowed.
 *  \ingroup Puppy
 */
void Puppy::initializePopulation(std::vector<Puppy::Tree>& ioPopulation,
                                 Puppy::Context& ioContext,
                                 float inInitGrowProba,
                                 unsigned int inMinDepth,
                                 unsigned int inMaxDepth)
{
  assert(inMinDepth <= inMaxDepth);
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    ioPopulation[i].resize(0);
    ioPopulation[i].mValid = false;
    unsigned int lInitDepth = ioContext.mRandom.rollInteger(inMinDepth, inMaxDepth);
    if(ioContext.mRandom.rollUniform() >= inInitGrowProba) {
      initializeTreeFull(ioPopulation[i], ioContext, lInitDepth);
    }
    else initializeTreeGrow(ioPopulation[i], ioContext, inMinDepth, lInitDepth);
  }
}


/*!
 *  \brief Initialize a GP tree with full approach.
 *  \param ioTree Tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \param inDepth Actual depth to go in initialization.
 *  \return Generated tree size.
 *  \ingroup Puppy
 *
 *  If the tree is not empty, the initialization append the generated sub-tree to the actual tree.
 */
unsigned int Puppy::initializeTreeFull(Puppy::Tree& ioTree,
                                       Puppy::Context& ioContext,
                                       unsigned int inDepth)
{
  assert(inDepth >= 1);
  if(inDepth == 1) {
    assert(ioContext.mTerminalSet.size() > 0);
    PrimitiveHandle lTerminal =
      ioContext.mTerminalSet[ioContext.mRandom(ioContext.mTerminalSet.size())];
    ioTree.push_back(Node(lTerminal->giveReference(ioContext), 1));
    return 1;
  }

  assert(ioContext.mFunctionSet.size() > 0);
  PrimitiveHandle lFunction =
    ioContext.mFunctionSet[ioContext.mRandom(ioContext.mFunctionSet.size())];

  unsigned int lNodeIndex = ioTree.size();
  ioTree.push_back(Node(lFunction->giveReference(ioContext), 0));
  unsigned int lNbArgs = ioTree[lNodeIndex].mPrimitive->getNumberArguments();
  unsigned int lTreeSize = 1;
  for(unsigned int i=0; i<lNbArgs; ++i) {
    lTreeSize += initializeTreeFull(ioTree, ioContext, inDepth-1);
  }
  ioTree[lNodeIndex].mSubTreeSize = lTreeSize;
  return lTreeSize;
}


/*!
 *  \brief Initialize a GP tree with grow approach.
 *  \param ioTree Tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \param inMinDepth Minimal depth to go in initialization.
 *  \param inMaxDepth Maximal depth to go in initialization.
 *  \return Generated tree size.
 *  \ingroup Puppy
 *
 *  If the tree is not empty, the initialization append the generated sub-tree to the actual tree.
 */
unsigned int Puppy::initializeTreeGrow(Puppy::Tree& ioTree,
                                       Puppy::Context& ioContext,
                                       unsigned int inMinDepth,
                                       unsigned int inMaxDepth)
{
  assert(inMinDepth >= 1);
  assert(inMinDepth <= inMaxDepth);

  PrimitiveHandle lPrimit = NULL;
  if(inMinDepth > 1) {
    assert(ioContext.mFunctionSet.size() > 0);
    lPrimit = ioContext.mFunctionSet[ioContext.mRandom(ioContext.mFunctionSet.size())];
  }
  else if(inMaxDepth == 1) {
    assert(ioContext.mTerminalSet.size() > 0);
    lPrimit = ioContext.mTerminalSet[ioContext.mRandom(ioContext.mTerminalSet.size())];
  }
  else {
    unsigned int lIndexSel =
      ioContext.mRandom(ioContext.mFunctionSet.size() + ioContext.mTerminalSet.size());
    if(lIndexSel >= ioContext.mFunctionSet.size()) {
      lPrimit = ioContext.mTerminalSet[lIndexSel - ioContext.mFunctionSet.size()];
    }
    else lPrimit = ioContext.mFunctionSet[lIndexSel];
  }

  unsigned int lNodeIndex = ioTree.size();
  ioTree.push_back(Node(lPrimit->giveReference(ioContext), 1));
  unsigned int lTreeSize = 1;
  unsigned int lMinDepth = (inMinDepth > 1) ? (inMinDepth-1) : 1;
  unsigned int lNbArgs = ioTree[lNodeIndex].mPrimitive->getNumberArguments();
  for(unsigned int i=0; i<lNbArgs; ++i) {
    lTreeSize += initializeTreeGrow(ioTree, ioContext, lMinDepth, inMaxDepth-1);
  }
  ioTree[lNodeIndex].mSubTreeSize = lTreeSize;
  return lTreeSize;
}



/*!
 *  \brief Apply standard (Koza's) mutation to a population of GP trees.
 *  \param ioPopulation Population to mutate.
 *  \param ioContext Evolutionary context.
 *  \param inMutationProba Mutation probability.
 *  \param inMaxRegenDepth Maximum tree regeneration depth allowed.
 *  \param inMaxDepth Maximum tree depth allowed.
 *  \ingroup Puppy
 */
void Puppy::applyMutationStandard(std::vector<Puppy::Tree>& ioPopulation,
                                  Puppy::Context& ioContext,
                                  float inMutationProba,
                                  unsigned int inMaxRegenDepth,
                                  unsigned int inMaxDepth)
{
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    if(ioContext.mRandom.rollUniform() < inMutationProba) {
      mutateStandard(ioPopulation[i], ioContext, inMaxRegenDepth, inMaxDepth);
    }
  }
}


/*!
 *  \brief Apply standard (Koza's) mutation on a GP trees.
 *  \param ioTree GP tree to mutate.
 *  \param ioContext Evolutionary context.
 *  \param inMaxRegenDepth Maximum tree regeneration depth allowed.
 *  \param inMaxDepth Maximum tree depth allowed.
 *  \ingroup Puppy
 */
void Puppy::mutateStandard(Puppy::Tree& ioTree,
                           Puppy::Context& ioContext,
                           unsigned int inMaxRegenDepth,
                           unsigned int inMaxDepth)
{
  assert(ioTree.size() > 0);
  unsigned int lMutIndex = ioContext.mRandom(ioTree.size());
  Tree lNewTree;
  lNewTree.insert(lNewTree.end(), ioTree.begin(), ioTree.begin()+lMutIndex);
  std::vector<unsigned int> lStack;
  ioTree.setStackToNode(lMutIndex, lStack);
  lStack.pop_back();
  unsigned int lTreeDepth = ioContext.mRandom.rollInteger(1, inMaxRegenDepth);
  unsigned int lTreeDepth2 = inMaxDepth - lStack.size();
  if(lTreeDepth2 < lTreeDepth) lTreeDepth = lTreeDepth2;
  assert(lTreeDepth > 0);
  initializeTreeGrow(lNewTree, ioContext, 1, lTreeDepth);
  lNewTree.insert(lNewTree.end(),
                  ioTree.begin()+lMutIndex+ioTree[lMutIndex].mSubTreeSize,
                  ioTree.end());
  unsigned int lDiffSize =
    ioTree[lMutIndex].mSubTreeSize - lNewTree[lMutIndex].mSubTreeSize;
  for(unsigned int i=0; i<lStack.size(); ++i) lNewTree[lStack[i]].mSubTreeSize -= lDiffSize;
  ioTree = lNewTree;
  ioTree.mValid = false;
}


/*!
 *  \brief Apply swap point mutation to a population of GP trees.
 *  \param ioPopulation Population to mutate.
 *  \param ioContext Evolutionary context.
 *  \param inMutationProba Mutation probability.
 *  \param inDistribProba Probability to mutate a function node, in opposition to a terminal.
 *  \ingroup Puppy
 */
void Puppy::applyMutationSwap(std::vector<Puppy::Tree>& ioPopulation,
                              Puppy::Context& ioContext,
                              float inMutationProba,
                              float inDistribProba)
{
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    if(ioContext.mRandom.rollUniform() < inMutationProba) {
      mutateSwap(ioPopulation[i], ioContext, inDistribProba);
    }
  }
}


/*!
 *  \brief Swap mutate a GP tree.
 *  \param ioTree GP tree to mutate.
 *  \param ioContext Evolutionary context.
 *  \param inDistribProba Probability to mutate a function node, in opposition to a terminal.
 *  \ingroup Puppy
 */
void Puppy::mutateSwap(Puppy::Tree& ioTree,
                       Puppy::Context& ioContext,
                       float inDistribProba)
{
  assert(ioTree.size() > 0);
  unsigned int lMutIndex = ioContext.mRandom(ioTree.size());
  if(ioTree.size() > 1) {
    bool lType = (ioContext.mRandom.rollUniform() < inDistribProba);
    while((ioTree[lMutIndex].mPrimitive->getNumberArguments() != 0) != lType) {
      lMutIndex = ioContext.mRandom(ioTree.size());
    }
  }

  unsigned int lNbArgs = ioTree[lMutIndex].mPrimitive->getNumberArguments();
  if(lNbArgs == 0) {
    assert(ioContext.mTerminalSet.size() > 0);
    PrimitiveHandle lTerminal =
      ioContext.mTerminalSet[ioContext.mRandom(ioContext.mTerminalSet.size())];
    ioTree[lMutIndex].mPrimitive = lTerminal->giveReference(ioContext);
  }
  else {
    std::vector<unsigned int> lKArgsFunction;
    for(unsigned int i=0; i<ioContext.mFunctionSet.size(); ++i) {
      if(ioContext.mFunctionSet[i]->getNumberArguments() == lNbArgs) {
        lKArgsFunction.push_back(i);
      }
    }
    assert(lKArgsFunction.size() > 0);
    PrimitiveHandle lFunction =
      ioContext.mFunctionSet[lKArgsFunction[ioContext.mRandom(lKArgsFunction.size())]];
    ioTree[lMutIndex].mPrimitive = lFunction->giveReference(ioContext);
  }
}


/*!
 *  \brief Apply tournament selection to a population of trees.
 *  \param ioPopulation Population to apply selection on.
 *  \param ioContext Evolutionary context.
 *  \param inNumberParticipants Number of participants to each tournament selection.
 *  \ingroup Puppy
 */
void Puppy::applySelectionTournament(std::vector<Puppy::Tree>& ioPopulation,
                                     Puppy::Context& ioContext,
                                     unsigned int inNumberParticipants)
{
  if(ioPopulation.size() == 0) return;

  std::vector<unsigned int> lIndices(ioPopulation.size(), 0);
  for(unsigned int i=0; i<ioPopulation.size(); i++) {
    unsigned int lChoosenIndividual = ioContext.mRandom(ioPopulation.size());
    for(unsigned int j=1; j<inNumberParticipants; ++j) {
      unsigned int lTriedIndividual = ioContext.mRandom(ioPopulation.size());
      if(ioPopulation[lChoosenIndividual] < ioPopulation[lTriedIndividual]) {
        lChoosenIndividual = lTriedIndividual;
      }
    }
    ++lIndices[lChoosenIndividual];
  }

  unsigned int lNextEmpty  = 0;
  unsigned int lNextFilled = 0;
  while((lNextFilled < ioPopulation.size()) && (lIndices[lNextFilled] <= 1)) lNextFilled++;
  while(lNextFilled < ioPopulation.size()) {
    while(lIndices[lNextFilled] > 1) {
      while(lIndices[lNextEmpty] != 0) ++lNextEmpty;
      ioPopulation[lNextEmpty] = ioPopulation[lNextFilled];
      --lIndices[lNextFilled];
      ++lIndices[lNextEmpty];
    }
    while((lNextFilled < ioPopulation.size()) && (lIndices[lNextFilled] <= 1)) ++lNextFilled;
  }
}


/*!
 *  \brief Apply roulette proportional selection to a population of trees.
 *  \param ioPopulation Population to apply selection on.
 *  \param ioContext Evolutionary context.
 *  \ingroup Puppy
 */
void Puppy::applySelectionRoulette(std::vector<Puppy::Tree>& ioPopulation,
                                   Puppy::Context& ioContext)
{
  if(ioPopulation.size() == 0) return;

  std::vector<float> lRoulette(ioPopulation.size());
  assert(ioPopulation[0].mValid);
  lRoulette[0] = ioPopulation[0].mFitness;
  for(unsigned int i=1; i<ioPopulation.size(); ++i) {
    assert(ioPopulation[i].mValid);
    lRoulette[i] = lRoulette[i-1] + ioPopulation[i].mFitness;
  }

  std::vector<unsigned int> lIndices(ioPopulation.size(), 0);
  for(unsigned int i=0; i<ioPopulation.size(); ++i) {
    float lDice = ioContext.mRandom.rollUniform(0.0, lRoulette.back());
    unsigned int lChoosen = 0;
    while(lDice < lRoulette[lChoosen]) ++lChoosen;
    ++lIndices[lChoosen];
  }

  unsigned int lNextEmpty  = 0;
  unsigned int lNextFilled = 0;
  while((lNextFilled < ioPopulation.size()) && (lIndices[lNextFilled] <= 1)) lNextFilled++;
  while(lNextFilled < ioPopulation.size()) {
    while(lIndices[lNextFilled] > 1) {
      while(lIndices[lNextEmpty] != 0) ++lNextEmpty;
      ioPopulation[lNextEmpty] = ioPopulation[lNextFilled];
      --lIndices[lNextFilled];
      ++lIndices[lNextEmpty];
    }
    while((lNextFilled < ioPopulation.size()) && (lIndices[lNextFilled] <= 1)) ++lNextFilled;
  }
}
