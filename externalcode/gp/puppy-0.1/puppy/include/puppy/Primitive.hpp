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
 *  \file   puppy/Primitive.hpp
 *  \brief  Definition of the class Primitive.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/11 15:46:20 $
 */

#ifndef Puppy_Primitive_hpp
#define Puppy_Primitive_hpp

#include <string>

#include "puppy/PrimitiveHandle.hpp"


namespace Puppy {

class Context;  // Forward declaration.

/*!
 *  \class Primitive puppy/Primitive.hpp "puppy/Primitive.hpp"
 *  \brief Genetic programming abstract primitive class.
 *  \ingroup Puppy
 *
 *  A primitive is an abstract class of terminals and functions that can be used
 *  in a GP program. Concrete primitives must define method execute, which define
 *  the characteristic operation to execute. Primitives are generally heap allocated
 *  (with a call to the new operator) and managed by the associated smart pointer
 *  defined in class PrimitiveHandle. 
 */
class Primitive {

public:

  explicit Primitive(unsigned int inNumberArguments=0, std::string inName="");
           Primitive(const Primitive& inRightPrimit);
  virtual ~Primitive() { }

  Primitive& operator=(const Primitive& inRightPrimit);
  
  /*!
   *  \brief Execute the caracteristic primitive operation.
   *  \param outDatum Result of the execution.
   *  \param ioContext Evolutionary context.
   */
  virtual void execute(void* outDatum, Context& ioContext) =0;

  inline  std::string     getName() const;
  inline  unsigned int    getNumberArguments() const;
  virtual PrimitiveHandle giveReference(Context& ioContext);
  inline  Primitive*      refer();
  virtual void            setValue(const void* inValue);
  inline  void            unrefer();
  
protected:

  inline void getArgument(unsigned int inN, void* outResult, Context& ioContext);
         void setName(std::string inName);
         void setNumberArguments(unsigned int inNumberArguments);
  
private:

  std::string  mName;             //!< Name of the primitive.
  unsigned int mNumberArguments;  //!< Number of arguments of the primitive.
  unsigned int mRefCounter;       //!< Primitive's reference counter.
   
};

}


#endif // Puppy_Primitive_hpp


 
