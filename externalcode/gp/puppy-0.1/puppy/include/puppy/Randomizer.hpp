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
 *  \file   puppy/Randomizer.hpp
 *  \brief  Definition of the class Randomizer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/11 18:38:13 $
 */

#ifndef Puppy_Randomizer_hpp
#define Puppy_Randomizer_hpp

#include <cassert>
#include <cstdlib>
#include <ctime>


namespace Puppy {

/*!
 *  \class Randomizer puppy/Randomizer.hpp "puppy/Randomizer.hpp"
 *  \brief Random number generator. C++ wrapper over standard C rand and srand functions.
 *  \ingroup Puppy
 *  \par Note:
 *    The operator() allow compliance with the STL random number generator interface.
 */
class Randomizer {

public:

  Randomizer() { }

  inline unsigned long operator()(unsigned long inN);

  inline long   rollInteger(long inLow, long inUp);
  inline double rollUniform(double inLow=0.0, double inUp=1.0);
  inline void   seed(unsigned long inSeed=0);

};

}


/*!
 *  \brief Generate a randomly generated integer in range [0,inN).
 *  \return Randomly generated integer.
 */
inline unsigned long Puppy::Randomizer::operator()(unsigned long inN)
{
  assert(inN > 0);
  assert(inN <= RAND_MAX);
  return (unsigned long)(double(inN) * double(std::rand()) / double(RAND_MAX+1.0));
}


/*!
 *  \brief Generate a randomly generated integer in range [inLow,inUp].
 *  \param inLow Lower bound of generation range.
 *  \param inUp Upper bound of generation range.
 *  \return Randomly generated integer.
 */
inline long Puppy::Randomizer::rollInteger(long inLow, long inUp)
{
  assert(inLow <= inUp);
  assert((inUp-inLow) < RAND_MAX);
  return inLow + (long)(double(inUp-inLow+1.0) * double(std::rand()) / double(RAND_MAX+1.0));
}


/*!
 *  \brief Generate a uniformly generated random real number in range [inLow,inUp).
 *  \param inLow Lower bound of generation range.
 *  \param inUp Upper bound of generation range.
 *  \return Randomly generated real number.
 */
inline double Puppy::Randomizer::rollUniform(double inLow, double inUp)
{
  assert(inLow <= inUp);
  return inLow + ((inUp-inLow) * double(std::rand()) / double(RAND_MAX+1.0));
}


/*!
 *  \brief Initialize the random number generator.
 *  \param inSeed Seed to use. If zero, CPU clock value is used.
 *  \return Randomly generated integer.
 */
inline void Puppy::Randomizer::seed(unsigned long inSeed)
{
  if(inSeed == 0) std::srand((unsigned int)std::time(NULL));
  else std::srand(inSeed);
}


#endif // Puppy_Randomizer_hpp
