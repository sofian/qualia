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

/*
 NOTE: This section was directly copied from file GAGenome.h in GAlib
       Copyright Massachusetts Institute of Technology and Matthew Wall.
 TODO: - adapt the text to the Qualia GA API

initialize
  Use this method to set the initial state of your genomes once they have
  been created.  This initialization is for setting up the genome's state,
  not for doing the basic mechanics of genome class management.  The
  default behaviour of this method is to change randomly the contents of the
  genome.  If you want to bias your initial population, this is where to
  make that happen.
     The initializer is used to initialize the genome (duh).  Notice that the
  state of the genome is unknown - memory may or may not have been allocated,
  and the genome may or may not have been used before.  So your initializer
  should first clean up as needed, then do its thing.  The initializer may be
  called any number of times (unlike a class constructor which is called only
  once for a given instance).

mutate
  Mutate the genome with probability as specified.  What mutation means
  depends upon the data type of the genome.  For example, you could have
  a bit string in which 50% mutation means each bit has a 50% chance of
  getting flipped, or you could have a tree in which 50% mutation means each
  node has a 50% chance of getting deleted, or you could have a bit string
  in which 50% mutation means 50% of the bits ACTUALLY get flipped.
    The mutations member returns the number of mutations since the genome
  was initialized.
    The mutator makes a change to the genome with likeliehood determined by the
  mutation rate parameter.  The exact meaning of mutation is up to you, as is
  the specific meaning of the mutation rate.  The function returns the number
  of mutations that actually occurred.

crossover
  Genomes don't really have any clue about other genomes, so we don't make
  the crossover a member function.  Instead, each genome kind of knows how
  to mate with other genomes to generate offspring, but they are not
  capable of doing it themselves.  The crossover member function is used to
  set the default mating mode for the genomes - it does not actually perform
  the crossover.  This way the GA can use asexual crossover if it wants to
  (but genomes only know how to do the default sexual crossover).
    This also lets you do funky stuff like crossover between different data
  types and group sex to generate new offspring.
     We define two types of crossover:  sexual and asexual.  Most GAlib
  algorithms use the sexual crossover, but both are available.  Each genome
  knows the preferred crossover method, but none is capable of reproducing.
  The genetic algorithm must actually perform the mating because it involves
  another genome (as parent and/or child).

evaluator
  Set the genome's objective function.  This also sets marks the evaluated
  flag to indicate that the genome must be re-evaluated.
    Evaluation happens on-demand - the objective score is not calculated until
  it is requested.  Then it is cached so that it does not need to be re-
  calculated each time it is requested.  This means that any member function
  that modifies the state of the genome must also set the evaluated flag to
  indicate that the score must be recalculated.
    The genome objective function is used by the GA to evaluate each member of
  the population.

comparator
  This method is used to determine how similar two genomes are.  If you want
  to use a different comparison method without deriving a new class, then use
  the comparator function to do so.  For example, you may want to do phenotype-
  based comparisons rather than genotype-based comparisons.
    In many cases we have to compare two genomes to determine how similar or
  different they are.  In traditional GA literature this type of function is
  referred to as a 'distance' function, probably because bit strings can be
  compared using the Hamming distance as a measure of similarity.  In GAlib, we
  define a genome comparator function that does exactly this kind of
  comparison.
    If the genomes are identical, the similarity function should return a
  value of 0.0, if completely different then return a value greater than 0.
  The specific definition of what "the same" and what "different" mean is up
  to you.  Most of the default comparators use the genotype for the comparison,
  but you can use the phenotype if you prefer.  There is no upper limit to the
  distance score as far as GAlib is concerned.
    The no-op function returns a -1 to signify that the comparison failed.

//evalData
//  The evalData member is useful if you do not want to derive a new genome class
//  but want to store data with each genome.  When you clone a genome, the eval
//  data also gets cloned so that each genome has its own eval data (unlike the
//  user data pointer described next which is shared by all genomes).
//
//userData
//  The userData member is used to provide all genomes access to the same user
//  data.  This can be a pointer to anything you want.  Any genome cloned from
//  another will share the same userData as the original.  This means that all
//  of the genomes in a population, for example, share the same userData.
//
//score
//  Evaluate the 'performance' of the genome using the objective function.
//  The score is kept in the 'score' member.  The 'evaluated' member tells us
//  whether or not we can trust the score.  Be sure to set/unset the 'evaluated'
//  member as appropriate (eg cross and mutate change the contents of the
//  genome so they unset the 'evaluated' flag).
//    If there is no objective function, then simply return the score.  This
//  allows us to use population-based evaluation methods (where the population
//  method sets the score of each genome).
*/

typedef float (*Evaluator) (Chromosome &);
typedef void  (*Initializer) (Chromosome &);
typedef void  (*Mutator) (Chromosome &, float);
typedef int   (*Comparator) (const Chromosome&, const Chromosome&);
typedef void  (*SexualCrossover) (const Chromosome&, const Chromosome&,
                                  Chromosome*, Chromosome*);
typedef void  (*AsexualCrossover) (const Chromosome&, Chromosome*);

class Chromosome {
public:
  Chromosome() {}
  //Chromosome(const Chromosome& c) { copyFrom(c); }
  virtual ~Chromosome() {}

  virtual void copyFrom(const Chromosome& c) { (*this) = c; }

  virtual void init(){}
  virtual void mutate(float p){}
  virtual bool equals(const Chromosome& g) const = 0;
//  virtual float evaluate() { return 0; }

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
