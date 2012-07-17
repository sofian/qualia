/*
 * Chromosome.h
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


#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

class Chromosome;

typedef float (*Evaluator) (Chromosome &);
typedef void  (*Initializer) (Chromosome &);
typedef void  (*Mutator) (Chromosome &, float);
typedef int   (*Comparator) (const Chromosome&, const Chromosome&);
typedef void  (*SexualCrossover) (const Chromosome&, const Chromosome&,
                                  Chromosome*, Chromosome*);
typedef void  (*AsexualCrossover) (const Chromosome&, Chromosome*);

class Chromosome {
public:
  int nGenes; // number of genes

  Chromosome(int nGenes_) : nGenes(nGenes_) {}
  Chromosome(const GAGenome& c) { copy(c); }
  virtual ~Chromosome() {}

  virtual void copy(const Chromosome& c) {}

  virtual void init(){}
  virtual void mutate(float p){}
  virtual int compare(const Chromosome& g) const { return (&g==this ? 0 : 1); }
  virtual float evaluate() { return 0; }

//  float evaluate() const { return (*evaluator)(*this); }
////  Evaluator evaluator() const { return eval; }
////  Evaluator evaluator(Evaluator f) { return (eval=f); }
//
//  void init() { (*initializer)(*this); }
////  Initializer initializer() const { return init; }
////  Initializer initializer(Initializer op){ return (init=op); }
//
//  void mutate(float p) { (*mutator)(*this,p); }
////  Mutator mutator() const { return mutator; }
////  Mutator setMutator(Mutator op){ return (mutator=op); }
//
//  float compare(const Chromosome& g) const { return (*comparator)(*this, g); }
////  Comparator comparator() const { return comparator; }
////  Comparator comparator(Comparator c){ return (comparator=c); }

};

#endif /* CHROMOSOME_H_ */
