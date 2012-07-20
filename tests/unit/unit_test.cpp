/*
 * unit_test.cpp
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
#include <stdio.h>

#include <qualia/core/common.h>
#include <qualia/core/Action.h>
#include <qualia/core/Observation.h>

#include <qualia/rl/RLObservation.h>

#include <qualia/rl/Policy.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>
#include <qualia/rl/QLearningSoftmaxPolicy.h>
#include <qualia/rl/RewardEnvironment.h>
#include <qualia/rl/Reward.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/util/random.h>
#include <qualia/util/bits.h>

#include <qualia/ga/BinaryChromosome.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

const unsigned int nActions[] = {3, 2, 2};
void testActions() {
  printf("== TEST ACTIONS ==\n");
  Action test(3, nActions);
  printf("- Testing conflated\n");
  assert( test.nConflated == 3*2*2 );
  for (action_t i=0; i<test.nConflated; i++) {
    action_t conf = test.setConflated(i).conflated();
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, conf, test[0], test[1], test[2] );
    assert( conf == i );
  }
  printf("-> PASSED\n");

  printf("- Testing iteration\n");
  test.reset();
  for (action_t i=0; test.hasNext(); test.next(), i++) {
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, test.conflated(), test[0], test[1], test[2] );
    assert( i < test.nConflated );
    assert( test.conflated() == i );
  }
  printf("-> PASSED\n");
}

const observation_t observation[]  = {1.0f, 2.0f, 3.0f};
const observation_t observation2[] = {2.0f, 3.0f, 4.0f};

void testObservations() {
  printf("== TEST OBSERVATIONS ==\n");
  printf("- Testing construction\n");
  RLObservation obs(3, observation);
  RLObservation obs2(3, observation2);
  printf("-> PASSED\n");

  printf("- Testing [] operator\n");
  for (int i=0; i<3; i++) {
    printf("%f %f\n", obs.observations[i], observation[i]);
    assert( obs[i] == observation[i] );
  }
  printf("-> PASSED\n");

  printf("- Testing copyFrom\n");
  obs.copyFrom(&obs2);
  for (int i=0; i<3; i++)
    assert( obs[i] == observation2[i] );
  printf("-> PASSED\n");

}

void testPolicies() {
  printf("== TEST POLICIES ==\n");
  Action action(2, (const unsigned int[]){10, 10});
  RLObservation observation(1);
  NeuralNetwork net(3, 3, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.1f);
  QLearningSoftmaxPolicy softmax;
  QLearningAgent agent(&net,
                       1, 2, (const unsigned int[]){10, 10},
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history


  printf("- Testing egreedy\n");
  egreedy.setAgent(&agent);

  printf("-- Testing greedy policy (epsilon = 0)\n");
  srandom(222);
  egreedy.epsilon = 0;
  egreedy.chooseAction(&action, &observation);
  // Actions should always be the same.
  action_t a = action.conflated();
  for (int i=0; i<100; i++) {
    egreedy.chooseAction(&action, &observation);
    assert( a == action.conflated() );
  }
  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1) (actions should be random)\n");
  srandom(222);
  const action_dim_t pattern1[] = {
      90, 90, 20, 97, 98, 30, 63, 23, 47, 32
  };
  egreedy.epsilon = 1;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    assert( action.conflated() == pattern1[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  srandom(222);
  const action_dim_t pattern2[] = {
      90, 9, 9, 9, 63, 47, 9, 9, 9, 9
  };
  egreedy.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
//    printf("%d, ", action.conflated());
    assert( action.conflated() == pattern2[i]);
  }
  printf("-> PASSED\n");

  printf("- Testing softmax\n");
  agent.policy = &softmax;
  softmax.setAgent(&agent);

  printf("-- Testing \"greedy\" policy (epsilon = 0)\n");
  srandom(222);
  const action_dim_t pattern3[] = {
      17, 66, 53, 82, 49, 61, 46, 28, 75, 56
  };
  softmax.epsilon = 0;
  softmax.chooseAction(&action, &observation);
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    assert( action.conflated() == pattern3[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1)\n");
  srandom(222);
  const action_dim_t pattern4[] = {
      90, 90, 20, 97, 98, 30, 63, 23, 47, 32
  };
  softmax.epsilon = 1;
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    assert( action.conflated() == pattern4[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  srandom(222);
  const action_dim_t pattern5[] = {
      90, 53, 49, 46, 32, 90, 94, 98, 61, 4
  };
  softmax.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
//    printf("%d, ", action.conflated());
    assert( action.conflated() == pattern5[i]);
  }
  printf("-> PASSED\n");

}

class TestEnvironment : public Environment {
public:
  RLObservation obs;
  TestEnvironment() : obs(2) {}
  virtual Observation* start() {
    obs[0] = obs[1] = 0;
    return &obs;
  }

  virtual Observation* step(const Action* action) {
    obs[0] = action->actions[0] / 10.0;
    obs[1] = action->actions[1] / 10.0;
    obs.reward = obs[0] + obs[1];
    return &obs;
  }
};

void realArrayToString(char* str, int n, real* p) {
  str[0] = '\0';
  for (int i=0; i<n; i++)
    sprintf(str, "%s%f, ", str, p[i]);
}

void testLearning() {
  printf("== TEST LEARNING ==\n");
  srandom(222);
  TestEnvironment env;
  NeuralNetwork net(2+2, 3, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.5f);
  QLearningAgent agent(&net,
                       2, 2, (const unsigned int[]){10, 10},
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history
  RLQualia qualia(&agent, &env);
  printf("-- Testing learning loop\n");
  qualia.init();
  qualia.start();

  // Verify weights
  const char* weights1 = "0.744364, 0.002764, 0.976832, 0.922620, -0.371778, -0.170730, 0.666554, -0.042378, -0.327445, -0.088207, -0.870008, -0.643873, -0.011151, -0.509864, 0.682508, -0.935150, -0.855299, -0.330070, 0.295997, ";
  const char* weights2 = "0.720454, -0.032911, 0.924488, 0.885604, -0.462369, -0.196202, 0.629109, -0.115963, -0.376370, -0.176181, -0.877519, -0.654348, -0.032678, -0.524523, 0.656541, -0.593092, -0.641793, -0.115928, 0.761823, ";

  char str[1000];
  realArrayToString(str, net.nParams, net.weights);
  assert( strcmp(weights1, str) == 0);

  qualia.episode(100);

  realArrayToString(str, net.nParams, net.weights);
  assert( strcmp(weights2, str) == 0);

  printf("-> PASSED\n");

}

void printBits(const uint8_t* toPrint, int size) {
  for (int i=0; i<size; i++) {
    for (int j=0; j<8; j++)
      printf("%d", (int)bitRead(toPrint[i], j));
    printf("|");
  }
  printf("\n");
}

void testBits() {
  printf("== TEST BIT MANIPULATIONS ==\n");
  long source;
  long source2;
  long dest;

  for (unsigned int len=0; len<=sizeof(long); len++) {
    for (unsigned int srcPos=0; srcPos<len; srcPos++) {
      dest = 0xffffffff;

      copyBits(&source2, &dest, srcPos, len, sizeof(long));
      for (unsigned int i=0; i<len; i++)
        assert( readBit((unsigned char*)&source2, i) == 1 );
      for (unsigned int i=len; i<32; i++)
        assert( readBit((unsigned char*)&source2, i) == 0 );

      for (unsigned int dstPos=0; dstPos<len; dstPos++) {
        source = 0;
        dest = 0xffffffff;
        writeBits(&source, &dest, srcPos, dstPos, len);

        assert( dest == 0xffffffff );

        for (unsigned int i=0; i<srcPos; i++)
          assert( readBit((unsigned char*)&source, i) == 0 );
        for (unsigned int i=srcPos; i<srcPos+len; i++)
          assert( readBit((unsigned char*)&source, i) == 1 );
        for (unsigned int i=srcPos+len; i<sizeof(long); i++)
          assert( readBit((unsigned char*)&source, i) == 0 );
      }
    }
  }

  for (unsigned int i=0; i<sizeof(long)*8; i++)
    clearBit((unsigned char*)&source, i);
  assert( source == 0 );
  printBits((unsigned char*)&source, sizeof(long));

  for (unsigned int i=0; i<sizeof(long)*8; i++)
    setBit((unsigned char*)&source, i);
  printBits((unsigned char*)&source, sizeof(long));
  assert( source == (long)0xffffffffffffffffL );

  for (unsigned int i=0; i<sizeof(long)*8; i++)
     flipBit((unsigned char*)&source, i);
  assert( source == 0 );

  printf("-> PASSED\n");
}

#define DUMMY_PARAMETERS_BITSIZE       61
#define DUMMY_PARAMETERS_BYTESIZE       8
#define DUMMY_PARAMETERS_TRAILING_BITS  3
#define DUMMY_PARAMETERS_N_GENES        9

struct DummyParameters {
  uint8_t value1bit :    1;
  uint8_t value2bit :    2;
  uint8_t value3bit :    3;
  uint8_t value4bit :    4;
  uint8_t value6bit :    6;
  uint8_t value8bit;
  uint16_t value10bit : 10;
  uint32_t value22bit : 22;
  uint8_t value5bit :    5;

  void print() {
    printf("1bit:  %d\n",  value1bit);
    printf("2bit:  %d\n",  value2bit);
    printf("3bit:  %d\n",  value3bit);
    printf("4bit:  %d\n",  value4bit);
    printf("6bit:  %d\n",  value6bit);
    printf("8bit:  %d\n",  value8bit);
    printf("10bit: %d\n",  value10bit);
    printf("22bit: %d\n",  value22bit);
    printf("5bit: %d\n",   value5bit);
  }
  void copyFrom(const BinaryChromosome& c) {
    value1bit = (uint8_t)  c.getGeneValue(0);
    value2bit = (uint8_t)  c.getGeneValue(1);
    value3bit = (uint8_t)  c.getGeneValue(2);
    value4bit = (uint8_t)  c.getGeneValue(3);
    value6bit = (uint8_t)  c.getGeneValue(4);
    value8bit = (uint8_t)  c.getGeneValue(5);
    value10bit = (uint16_t)c.getGeneValue(6);
    value22bit = (uint32_t)c.getGeneValue(7);
    value5bit = (uint8_t)  c.getGeneValue(8);
    //memcpy((uint8_t*)this, code, DUMMY_PARAMETERS_BYTESIZE+1);
  }
};

void print(BinaryChromosome& c, bool justBits=false) {
  if (!justBits) {
    DummyParameters params;
    params.copyFrom(c);
    params.print();
  }
  printBits(c.code, c.info->byteSize());
}

void testBinaryChromosomes() {
  printf("== TEST BINARY CHROMOSOMES MANIPS ==\n");

  const uint8_t geneSizes[] = { 1, 2, 3, 4, 6, 8, 10, 22, 5 };
  const uint8_t geneSizes2[] = { 2, 2, 3, 4, 6, 8, 10, 22, 5 };
  BinaryChromosomeInfo info(DUMMY_PARAMETERS_N_GENES, geneSizes);

  printf("- Testing BitChromosomeInfo\n");

  printf("-- Testing bitSize() / byteSize()\n");
  printf("Info params: %d %d %d\n", info.nGenes, info.bitSize(), info.byteSize());
  assert( info.bitSize()  == DUMMY_PARAMETERS_BITSIZE );
  assert( info.byteSize() == DUMMY_PARAMETERS_BYTESIZE );
  assert( info.nGenes == DUMMY_PARAMETERS_N_GENES );
  printf("-> PASSED\n");

  printf("-- Testing equality operator\n");
  BinaryChromosomeInfo infoSame(9, geneSizes);
  BinaryChromosomeInfo infoWrong(9, geneSizes2);
  assert( info == infoSame );
  assert( info != infoWrong );
  printf("-> PASSED\n");

  printf("- Testing initializer\n");

  BinaryChromosome parent1(&info);
  BinaryChromosome parent2(&info);
  BinaryChromosome children1(&info);
  BinaryChromosome children2(&info);

  BinaryChromosome tmp(&info);

  printf("-- Testing fill-up with trailing zeros\n");
  parent1.init();
  for (int i=0; i<DUMMY_PARAMETERS_BITSIZE; i++)
    setBit((unsigned char*)parent1.code, i);
  for (int i=8-DUMMY_PARAMETERS_TRAILING_BITS; i<8; i++) {
    assert( bitRead(parent1.code[DUMMY_PARAMETERS_BYTESIZE-1], i) == 0 );
  }
  printf("-> PASSED\n");

  printf("Init parent1\n");
  parent1.init();
  print(parent1);

  printf("Init parent2\n");
  parent2.init();
  print(parent2);

  printf("- Test copy\n");
  tmp.copy(parent1);
  print(tmp);
  assert( memcmp(tmp.code, parent1.code, info.byteSize()) == 0);
  printf("-> PASSED\n");

  printf("- Test flip mutation\n");

  printf("-- Test flip mutate (prob=0) (ie. no mutations)\n");
  parent1.mutate(0);
  print(parent1);
  assert( memcmp(tmp.code, parent1.code, info.byteSize()) == 0);
  printf("-> PASSED\n");

  printf("--- Test flip mutate (prob=1) (ie. everything flipped)\n");
  parent1.mutate(1);
  print(parent1);
  for (unsigned int i=0; i<info.byteSize(); i++) {
    // Check that pre-mutated parent is the exact opposite of post-mutated parent (XOR)
    if (i < info.byteSize()-1)
      assert( (tmp.code[i] ^ parent1.code[i]) == 0xff );
    else // cope for trailing bits)
      assert( (tmp.code[i] ^ parent1.code[i]) == (0xff >> DUMMY_PARAMETERS_TRAILING_BITS) );
  }
  printf("-> PASSED\n");

  printf("- Check flip mutate (prob=0.5)\n");
  parent1.copy(tmp);
  parent1.mutate(0.5f);
  print(parent1);

  printf("- Test compare between chromosomes\n");

  printf("-- Test: chromosome equals itself\n");
  assert( parent1.compare(parent1) == 0 );
  assert( parent1 == parent1 );
  printf("-> PASSED\n");

  printf("-- Test: chromosome compare with different info that ==\n");
  BinaryChromosome parentSame(&infoSame);
  parentSame.copy(parent1);
  assert( parent1.compare(parentSame) == 0 );
  assert( parent1 == parentSame );
  printf("-> PASSED\n");

  printf("-- Test: chromosome compare with different info (should return -1 / fail)\n");
  BinaryChromosome parentWrong(&infoWrong);
  assert( parent1.compare(parentWrong) == -1 );
  assert( parent1 != parentWrong );
  printf("-> PASSED\n");

  printf("-- Test: chromosome compare with flip = n bits (all bits differ)\n");
  tmp.copy(parent1); // copy
  parent1.mutate(1);
  assert( parent1 != tmp );
  printf("Diff %d\n", parent1.compare(tmp));
  assert( parent1.compare(tmp) == DUMMY_PARAMETERS_BITSIZE );
  printf("-> PASSED\n");

  printf("- Testing crossovers\n");
  printf("-- Testing one point crossover\n");
  for (int i=0; i<DUMMY_PARAMETERS_BITSIZE; i++) {
    setBit(parent1.code, i);
    clearBit(parent2.code, i);
  }
  printf("Parents:\n");
  print(parent1);
  print(parent2);
  BinaryChromosome::crossoverOnePoint(parent1, parent2, &children1, &children2);
  printf("Children:\n");
  print(children1);
  print(children2);
  assert( children1.compare(children2) == DUMMY_PARAMETERS_BITSIZE);
  // Repeat a few times.
  for (int i=0; i<100; i++) {
    BinaryChromosome::crossoverOnePoint(parent1, parent2, &children1, &children2);
    assert( children1.compare(children2) == DUMMY_PARAMETERS_BITSIZE);
  }
  printf("-> PASSED\n");

  printf("-- Testing two point crossover\n");
  BinaryChromosome::crossoverTwoPoint(parent1, parent2, &children1, &children2);
  printf("Children:\n");
  print(children1);
  print(children2);
  assert( children1.compare(children2) == DUMMY_PARAMETERS_BITSIZE);
  // Repeat a few times.
  for (int i=0; i<100; i++) {
    BinaryChromosome::crossoverTwoPoint(parent1, parent2, &children1, &children2);
    assert( children1.compare(children2) == DUMMY_PARAMETERS_BITSIZE);
  }
  printf("-> PASSED\n");

  printf("- Testing gene access methods\n");

  printf("-- Testing getGeneValue()\n");

  for (int i=0; i<DUMMY_PARAMETERS_BITSIZE; i++) {
    setBit(parent1.code, i);   // all ones
    clearBit(parent2.code, i); // all zeros
  }
  for (int i=0; i<DUMMY_PARAMETERS_N_GENES; i++) {
    assert( parent1.getGeneValue(i) == (uint64_t) (pow(2,geneSizes[i])-1) );
    assert( parent2.getGeneValue(i) == 0 );
  }

  for (int i=0; i<100; i++) {
    parent1.init();
    tmp.copy(parent1);
    parent1.mutate(1); // flip all
    for (int i=0; i<DUMMY_PARAMETERS_N_GENES; i++) {
      // Check that pre-mutated gene is the exact opposite of post-mutated gene (XOR)
      assert( (parent1.getGeneValue(i) ^ tmp.getGeneValue(i)) == (uint64_t) (0xffffffffffffffffL >> (64-geneSizes[i])) );
    }
  }
  printf("-> PASSED\n");

  printf("-- Testing geneSetValue()\n");

  parent1.init();
  parent2.init();

  for (int i=0; i<DUMMY_PARAMETERS_N_GENES; i++) {
    uint64_t maxGeneValue = (uint64_t) pow(2, info.geneSizes[i])-1;
    // Don't go through all values cause it can be very long.
    uint64_t inc          = info.geneSizes[i] > 8 ? info.geneSizes[i]*11 : 1;
    for (uint64_t x=0; x<maxGeneValue-1; x += inc) {
      parent1.setGeneValue(i, x);
      assert( parent1.getGeneValue(i) == x);
    }
    parent1.setGeneValue(i, maxGeneValue);
    assert( parent1.getGeneValue(i) == maxGeneValue);
  }
  printf("-> PASSED\n");



}


int main() {
  testActions();
  testObservations();
  testPolicies();
  testLearning();
  testBits();
  testBinaryChromosomes();
}
