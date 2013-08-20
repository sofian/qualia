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

#include <qualia/learning/NeuralNetwork.h>

#include <qualia/core/FileExportEnvironment.h>
#include <qualia/learning/MemoryDataSet.h>
#include <qualia/rl/TupleDataSet.h>

#include <qualia/rl/RLObservation.h>
#include <qualia/rl/Policy.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QFunction.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>
#include <qualia/rl/QLearningSoftmaxPolicy.h>
#include <qualia/rl/RewardEnvironment.h>
#include <qualia/rl/Reward.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/util/random.h>
#include <qualia/util/bitarray.h>

#include <qualia/ga/BinaryChromosome.h>

#include <qualia/computer/DiskXFile.h>
#include <qualia/computer/DiskXFileDataSet.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

const unsigned int nActions[] = {3, 2, 2};
void testActions() {
  printf("== TEST ACTIONS ==\n");
  ActionProperties props(3, nActions);
  Action test(&props);
  printf("- Testing conflated\n");
  assert( test.nConflated() == 3*2*2 );
  for (action_t i=0; i<test.nConflated(); i++) {
    action_t conf = test.setConflated(i).conflated();
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, conf, test[0], test[1], test[2] );
    assert( conf == i );
  }
  printf("-> PASSED\n");

  printf("- Testing iteration\n");
  test.reset();
  Q_ASSERT_ERROR(test.undefined());
  action_t i = 0;
  while (test.hasNext()) {
    test.next();
    Q_ASSERT_ERROR(! test.undefined());
    Q_ASSERT_ERROR( i < test.nConflated() );
    Q_ASSERT_ERROR( test.conflated() == i );
    i++;
  }
  printf("-> PASSED\n");

  printf("- Testing copyFrom\n");
  Action testCopy(&props);
  i = 0;
  while (test.hasNext()) {
    test.next();
    testCopy.copyFrom(test);
    Q_ASSERT_ERROR( test.conflated() == testCopy.conflated() );
    i++;
  }
  printf("-> PASSED\n");
}

const observation_t observation[]  = {1.0f, 2.0f, 3.0f};
const observation_t observation2[] = {2.0f, 3.0f, 4.0f};

void testObservations() {
  printf("== TEST OBSERVATIONS ==\n");
  printf("- Testing construction\n");
  RLObservation obs(3);
  RLObservation obs2(3);
  for (int i=0; i<3; i++) {
    obs[i] = observation[i];
    obs2[i] = observation2[i];
  }
  printf("-> PASSED\n");

  printf("- Testing [] operator\n");
  for (int i=0; i<3; i++) {
    printf("%f %f\n", obs.observations[i], observation[i]);
    assert( obs[i] == observation[i] );
  }
  printf("-> PASSED\n");

  printf("- Testing copyFrom\n");
  obs.copyFrom(obs2);
  for (int i=0; i<3; i++)
    assert( obs[i] == observation2[i] );
  printf("-> PASSED\n");

}

void testPolicies() {
  printf("== TEST POLICIES ==\n");
  ActionProperties props(2, (const unsigned int[]){10, 10});
  Action action(&props);
  RLObservation observation(1);
  NeuralNetwork net(3, 3, 1, 0.1f, 0, 0, false);
  QFunction q(&net, 1, &props);
  QLearningEGreedyPolicy egreedy(0.1f);
  QLearningSoftmaxPolicy softmax;
  QLearningAgent agent(&q,
                       &egreedy,
                       1,
                       &props,
                       1.0f, 0.1f, false); // lambda = 1.0 => no history


  printf("- Testing egreedy\n");
  egreedy.setAgent(&agent);
  agent.init();

  printf("-- Testing greedy policy (epsilon = 0)\n");
  randomSeed(222);
  egreedy.epsilon = 0;
  egreedy.chooseAction(&action, &observation);
  // Actions should always be the same.
  action_t a = action.conflated();
  for (int i=0; i<100; i++) {
    egreedy.chooseAction(&action, &observation);
    Q_ASSERT_ERROR( a == action.conflated() );
  }
  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1) (actions should be random)\n");
  randomSeed(222);
  const action_dim_t pattern1[] = {
      90, 90, 20, 97, 98, 30, 63, 23, 47, 32
  };
  egreedy.epsilon = 1;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    //printf("%d, ", action.conflated());
    Q_ASSERT_ERROR( action.conflated() == pattern1[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  randomSeed(222);
  const action_dim_t pattern2[] = {
      90, 9, 9, 9, 63, 47, 9, 9, 9, 9
  };
  egreedy.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    //printf("%d, ", action.conflated());
    Q_ASSERT_ERROR( action.conflated() == pattern2[i] );
  }
  printf("-> PASSED\n");

  printf("- Testing softmax\n");
  agent.policy = &softmax;
  softmax.setAgent(&agent);

  printf("-- Testing pure softmax policy (epsilon = 0)\n");
  randomSeed(222);
  const action_dim_t pattern3[] = {
      17, 66, 53, 82, 49, 61, 46, 28, 75, 56
  };
  softmax.epsilon = 0;
  softmax.chooseAction(&action, &observation);
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    Q_ASSERT_ERROR( action.conflated() == pattern3[i]);
  }

  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1)\n");
  randomSeed(222);
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
  randomSeed(222);
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

void realArrayToString(char* str, int n, const real* p) {
  str[0] = '\0';
  for (int i=0; i<n; i++)
    sprintf(str, "%s%f, ", str, p[i]);
}

bool approxEqual(real x1, real x2, real diffThreshold=0.000001) {
  return (abs(x1-x2) < diffThreshold);
}

void testLearning() {
  printf("== TEST LEARNING ==\n");
  randomSeed(222);
  ActionProperties props(2, (const unsigned int[]){10, 10});
  TestEnvironment env;
  NeuralNetwork net(2+2, 3, 1, 0.001f, 1e-6, 1e-6, false);
  QFunction q(&net, 2, &props);
  QLearningEGreedyPolicy egreedy(0.5f);
  QLearningAgent agent(&q,
                       &egreedy,
                       2,
                       &props,
                       1.0f, 0.1f, false); // lambda = 1.0 => no history
  RLQualia qualia(&agent, &env);
  qualia.init();

  // Verify weights
  const real weights1[] = { -0.922944, -0.639856, 0.326380, 0.076001, 0.654564, -0.012578, 0.231147, -0.073863, -0.429572, 0.521223, 0.127283, 0.804040, 0.096144, 0.898829, 0.181497, 0.979205, 0.532769, 0.228138, -0.308403 };

  printf("-- Testing initialization\n");
  for (unsigned int i=0; i<net.nParams(); i++) {
    Q_ASSERT_ERROR( approxEqual(weights1[i], net.weights[i]));
  }

  printf("-> PASSED\n");

  printf("-- Testing learning loop without randomness\n");
  egreedy.epsilon = 0;

#if defined(__APPLE__)
  #warning "This test should be checked again on OSX"
  // Values as they were compiled on OSX.
#error "Need to recompile weights under OSX"
#else // Linux
const real weights2[] = { -0.868334, -0.591405, 0.387102, 0.123143, 0.746422, 0.015176, 0.265776, -0.046687, -0.386320, 0.528829, 0.136874, 0.811375, 0.105735, 0.898829, 0.181497, 1.087988, 0.671608, 0.428637, -0.071479, };
//  const real weights2[] = { 0.744364, 0.004321, 0.968882, 0.918303, -0.363828, -0.167727, 0.651167, -0.050757, -0.312058, -0.087158, -0.875340, -0.646770, -0.005819, -0.509864, 0.682508, -0.944954, -0.895760, -0.380126, 0.224388 };
#endif

  qualia.episode(1001); // does 1 call to start() and 1000 calls to step()

  for (unsigned int i=0; i<net.nParams(); i++) {
    Q_ASSERT_ERROR( approxEqual(weights2[i], net.weights[i]));
  }
  printf("-> PASSED\n");

  printf("-- Testing learning loop with 50%% randomness\n");
  egreedy.epsilon = 0.5;

#if defined(__APPLE__)
  #warning "This test should be checked again on OSX"
  // Values as they were compiled on OSX.
  const real weights3[] = { 0.720454, -0.032911, 0.924488, 0.885604, -0.462369, -0.196202, 0.629109, -0.115963, -0.376370, -0.176181, -0.877519, -0.654348, -0.032678, -0.524523, 0.656541, -0.593092, -0.641793, -0.115928, 0.761823 };
#else // Linux
  const real weights3[] = { -0.848086, -0.571461, 0.413322, 0.149828, 0.787547, 0.027555, 0.282516, -0.029533, -0.363809, 0.532818, 0.143041, 0.817368, 0.112163, 0.898829, 0.181497, 1.148808, 0.734514, 0.520290, 0.036034 };
//  const real weights3[] = { 0.728281, -0.031507, 0.934786, 0.890957, -0.472604, -0.203539, 0.604837, -0.086146, -0.406650, -0.098449, -0.890225, -0.658924, -0.035352, -0.509864, 0.682508, -0.663268, -0.671780, -0.143882, 0.683062 };
#endif

  qualia.episode(1001); // does 1 call to start() and 1000 calls to step()

  for (unsigned int i=0; i<net.nParams(); i++) {
//    printf("%f, ", net.weights[i]);
    Q_ASSERT_ERROR( approxEqual(weights3[i], net.weights[i]));
  }

  printf("-> PASSED\n");

}

#ifdef USE_DOUBLE
#define REAL_FORMAT "%lf"
#else
#define REAL_FORMAT "%f"
#endif

void testData() {
  printf("== TEST DATA ==\n");
  randomSeed(222);

  printf("-- Testing DiskXFile\n");

  DiskXFile file("testfile.txt", "w+");
  const char* TEST_STRING = "HELLOWORLD!";
  file.printf(TEST_STRING);
  file.rewind();
  char verify[1000];
  file.scanf("%s", verify);
  Q_ASSERT_ERROR( strcmp(verify, TEST_STRING) == 0 );

  file.rewind();
  int nExamples = 100;
  int dim       = 10;
  file.printf("%d %d\n", nExamples, dim);
  float k=0;
  for (int t=0; t<nExamples; t++) {
    for (int i=0; i<dim; i++) {
      file.printf(REAL_FORMAT, k++);
      file.printf(" ");
    }
    file.printf("\n");
  }

  file.rewind();
  int test;
  file.scanf("%d", &test);
  assert( test == nExamples );
  file.scanf("%d", &test);
  Q_ASSERT_ERROR( test == dim );

  k=0;
  for (int t=0; t<nExamples; t++) {
    for (int i=0; i<dim; i++) {
      float tmp;
      file.scanf(REAL_FORMAT, &tmp);
      Q_ASSERT_ERROR( tmp == k );
      k++;
    }
  }
  printf("-> PASSED\n");

  printf("-- Testing DiskXFileDataSet\n");
  DiskXFileDataSet dataset(&file, true);
  dataset.init();

  k=0;
  for (int t=0; t<nExamples; t++) {
    dataset.setExample(t);
    for (int i=0; i<dim; i++) {
      Q_ASSERT_ERROR_MESSAGE( dataset.example[i] == k, "Wrong value at example %d (%f != %f)", t, dataset.example[i], k);
      k++;
    }
  }
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

      arrayBlockCopy(&source2, &dest, srcPos, len, sizeof(long));
      for (unsigned int i=0; i<len; i++)
        assert( arrayBitRead((unsigned char*)&source2, i) == 1 );
      for (unsigned int i=len; i<32; i++)
        assert( arrayBitRead((unsigned char*)&source2, i) == 0 );

      for (unsigned int dstPos=0; dstPos<len; dstPos++) {
        source = 0;
        dest = 0xffffffff;
        arrayBlockWrite(&source, &dest, srcPos, dstPos, len);

        assert( dest == 0xffffffff );

        for (unsigned int i=0; i<srcPos; i++)
          assert( arrayBitRead((unsigned char*)&source, i) == 0 );
        for (unsigned int i=srcPos; i<srcPos+len; i++)
          assert( arrayBitRead((unsigned char*)&source, i) == 1 );
        for (unsigned int i=srcPos+len; i<sizeof(long); i++)
          assert( arrayBitRead((unsigned char*)&source, i) == 0 );
      }
    }
  }

  for (unsigned int i=0; i<sizeof(long)*8; i++)
    arrayBitClear((unsigned char*)&source, i);
  assert( source == 0 );
  printBits((unsigned char*)&source, sizeof(long));

  for (unsigned int i=0; i<sizeof(long)*8; i++)
    arrayBitSet((unsigned char*)&source, i);
  printBits((unsigned char*)&source, sizeof(long));
  assert( source == (long)0xffffffffffffffffL );

  for (unsigned int i=0; i<sizeof(long)*8; i++)
     arrayBitFlip((unsigned char*)&source, i);
  assert( source == 0 );

  printf("-> PASSED\n");
}

#define TEST_CHROMOSOME_BITSIZE       61
#define TEST_CHROMOSOME_BYTESIZE       8
#define TEST_CHROMOSOME_TRAILING_BITS  3
#define TEST_CHROMOSOME_N_GENES        9

struct TestChromosomeParams {
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
    //memcpy((uint8_t*)this, code, TEST_CHROMOSOME_BYTESIZE+1);
  }
};

void print(BinaryChromosome& c, bool justBits=false) {
  if (!justBits) {
    TestChromosomeParams params;
    params.copyFrom(c);
    params.print();
  }
  printBits(c.code, c.info->byteSize());
}

void testBinaryChromosomes() {
  printf("== TEST BINARY CHROMOSOMES MANIPS ==\n");

  const uint8_t geneSizes[] = { 1, 2, 3, 4, 6, 8, 10, 22, 5 };
  const uint8_t geneSizes2[] = { 2, 2, 3, 4, 6, 8, 10, 22, 5 };
  BinaryChromosomeProperties info(TEST_CHROMOSOME_N_GENES, geneSizes);

  printf("- Testing BitChromosomeInfo\n");

  printf("-- Testing bitSize() / byteSize()\n");
  printf("Info params: %d %d %d\n", info.nGenes(), info.bitSize(), info.byteSize());
  assert( info.bitSize()  == TEST_CHROMOSOME_BITSIZE );
  assert( info.byteSize() == TEST_CHROMOSOME_BYTESIZE );
  assert( info.nGenes() == TEST_CHROMOSOME_N_GENES );
  printf("-> PASSED\n");

  printf("-- Testing equality operator\n");
  BinaryChromosomeProperties infoSame(9, geneSizes);
  BinaryChromosomeProperties infoWrong(9, geneSizes2);
  Q_ASSERT_ERROR( info.equals(infoSame) );
  Q_ASSERT_ERROR( !info.equals(infoWrong) );
  printf("-> PASSED\n");

  printf("- Testing initializer\n");

  BinaryChromosome parent1(&info);
  BinaryChromosome parent2(&info);
  BinaryChromosome children1(&info);
  BinaryChromosome children2(&info);

  BinaryChromosome tmp(&info);

  printf("-- Testing fill-up with trailing zeros\n");
  parent1.init();
  for (int i=0; i<TEST_CHROMOSOME_BITSIZE; i++)
    arrayBitSet((unsigned char*)parent1.code, i);
  for (int i=8-TEST_CHROMOSOME_TRAILING_BITS; i<8; i++) {
    assert( bitRead(parent1.code[TEST_CHROMOSOME_BYTESIZE-1], i) == 0 );
  }
  printf("-> PASSED\n");

  printf("Init parent1\n");
  parent1.init();
  print(parent1);

  printf("Init parent2\n");
  parent2.init();
  print(parent2);

  printf("- Test copy\n");
  tmp.copyFrom(parent1);
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
      assert( (tmp.code[i] ^ parent1.code[i]) == (0xff >> TEST_CHROMOSOME_TRAILING_BITS) );
  }
  printf("-> PASSED\n");

  printf("- Check flip mutate (prob=0.5)\n");
  parent1.copyFrom(tmp);
  parent1.mutate(0.5f);
  print(parent1);

  printf("- Test compare between chromosomes\n");

  printf("-- Test: chromosome equals itself\n");
  Q_ASSERT_ERROR( parent1.equals(parent1) );
  printf("-> PASSED\n");

  printf("-- Test: chromosome compare with different info that ==\n");
  BinaryChromosome parentSame(&infoSame);
  parentSame.copyFrom(parent1);
  Q_ASSERT_ERROR( parent1.equals(parentSame) );
  //assert( parent1 == parentSame );
  printf("-> PASSED\n");

  printf("-- Test: chromosome compare with different info (should return -1 / fail)\n");
  BinaryChromosome parentWrong(&infoWrong);
  Q_ASSERT_ERROR( !parent1.equals(parentWrong) );
  printf("-> PASSED\n");

  printf("-- Test: chromosome compare with flip = n bits (all bits differ)\n");
  tmp.copyFrom(parent1); // copy
  parent1.mutate(1);
  Q_ASSERT_ERROR( !parent1.equals(tmp) );
  //printf("Diff %d\n", parent1.compareTo(tmp));
  //assert( parent1.compareTo(tmp) == TEST_CHROMOSOME_BITSIZE );
  printf("-> PASSED\n");

  printf("- Testing crossovers\n");
  printf("-- Testing one point crossover\n");
  for (int i=0; i<TEST_CHROMOSOME_BITSIZE; i++) {
    arrayBitSet(parent1.code, i);
    arrayBitClear(parent2.code, i);
  }
  printf("Parents:\n");
  print(parent1);
  print(parent2);
  BinaryChromosome::crossoverOnePoint(parent1, parent2, &children1, &children2);
  printf("Children:\n");
  print(children1);
  print(children2);
  //Q_ASSERT_ERROR( children1.compareTo(children2) == TEST_CHROMOSOME_BITSIZE);
  // Repeat a few times.
  for (int i=0; i<100; i++) {
    BinaryChromosome::crossoverOnePoint(parent1, parent2, &children1, &children2);
    //assert( children1.compareTo(children2) == TEST_CHROMOSOME_BITSIZE);
  }
  printf("-> PASSED\n");

  printf("-- Testing two point crossover\n");
  BinaryChromosome::crossoverTwoPoint(parent1, parent2, &children1, &children2);
  printf("Children:\n");
  print(children1);
  print(children2);
  //Q_ASSERT_ERROR( children1.compareTo(children2) == TEST_CHROMOSOME_BITSIZE);
  // Repeat a few times.
  for (int i=0; i<100; i++) {
    BinaryChromosome::crossoverTwoPoint(parent1, parent2, &children1, &children2);
    //assert( children1.compareTo(children2) == TEST_CHROMOSOME_BITSIZE);
  }
  printf("-> PASSED\n");

  printf("- Testing gene access methods\n");

  printf("-- Testing getGeneValue()\n");

  for (int i=0; i<TEST_CHROMOSOME_BITSIZE; i++) {
    arrayBitSet(parent1.code, i);   // all ones
    arrayBitClear(parent2.code, i); // all zeros
  }
  for (int i=0; i<TEST_CHROMOSOME_N_GENES; i++) {
    assert( parent1.getGeneValue(i) == (uint64_t) (pow(2,geneSizes[i])-1) );
    assert( parent2.getGeneValue(i) == 0 );
  }

  for (int i=0; i<100; i++) {
    parent1.init();
    tmp.copyFrom(parent1);
    parent1.mutate(1); // flip all
    for (int i=0; i<TEST_CHROMOSOME_N_GENES; i++) {
      // Check that pre-mutated gene is the exact opposite of post-mutated gene (XOR)
      assert( (parent1.getGeneValue(i) ^ tmp.getGeneValue(i)) == (uint64_t) (0xffffffffffffffffL >> (64-geneSizes[i])) );
    }
  }
  printf("-> PASSED\n");

  printf("-- Testing geneSetValue()\n");

  parent1.init();
  parent2.init();

  for (int i=0; i<TEST_CHROMOSOME_N_GENES; i++) {
    uint64_t maxGeneValue = (uint64_t) pow(2, info.geneSize(i))-1;
    // Don't go through all values cause it can be very long.
    uint64_t inc          = info.geneSize(i) > 8 ? info.geneSize(i)*11 : 1;
    for (uint64_t x=0; x<maxGeneValue-1; x += inc) {
      parent1.setGeneValue(i, x);
      assert( parent1.getGeneValue(i) == x);
    }
    parent1.setGeneValue(i, maxGeneValue);
    assert( parent1.getGeneValue(i) == maxGeneValue);
  }
  printf("-> PASSED\n");
}

void testDataSet() {
  printf("== TEST DATASET ==\n");
  randomSeed(222);
  unsigned int observationDim = 2;
  ActionProperties props(2, (const unsigned int[]){10, 10});

  printf("- Test generating RAW data\n");
  DiskXFile testFile("test_set.raw", "w+");
  TestEnvironment testEnv;
  FileExportEnvironment env(&testEnv, &testFile, observationDim, props.dim());

  NeuralNetwork net(observationDim+props.dim(), 3, 1, 0.1f, 0, 0, false);
  QFunction q(&net, observationDim, &props);
  QLearningEGreedyPolicy egreedy(0.5f);
  QLearningAgent agent(&q,
                       &egreedy,
                       observationDim,
                       &props,
                       1.0f, 0.1f, false); // lambda = 1.0 => no history

  // Iterate and generate data.
  RLQualia qualia(&agent, &env);

  randomSeed(222);
  qualia.init();
  qualia.episode(11);

  long expectedFileSize = sizeof(unsigned int)*2 + sizeof(observation_t)*(11*(observationDim+1)) + sizeof(action_dim_t)*10*props.dim();
  Q_ASSERT_ERROR( testFile.size() == expectedFileSize);

  printf("-> PASSED\n");

  printf("- Test reading and loading tuples from RAW file\n");
  RLQualia qualia2(&agent, &testEnv);

  TupleDataSet tuplesDataSet(&testFile, observationDim, &props);
  MemoryDataSet memDataSet(&tuplesDataSet);

  memDataSet.init();
  memDataSet.reset();

  randomSeed(222);
  qualia2.init();

  ObservationAction* oa = qualia2.start();
  RLObservation lastObs(observationDim);
  lastObs.copyFrom(*oa->observation);
  for (int t=0; t<memDataSet.nExamples(); t++) {
    memDataSet.setExample(t);

    // Data is (s, a, r, s')
    real* ptr = memDataSet.example;
    for (unsigned int i=0; i<observationDim; i++)
      Q_ASSERT_ERROR( lastObs[i] == *ptr++ );

    for (unsigned int i=0; i<props.dim(); i++)
      Q_ASSERT_ERROR( oa->action->actions[i] == (action_dim_t) (*ptr++) );

    oa = qualia2.step();

    Q_ASSERT_ERROR( ((RLObservation*)oa->observation)->reward == *ptr++ );

    for (unsigned int i=0; i<observationDim; i++)
      Q_ASSERT_ERROR( oa->observation->observations[i] == *ptr++ );

    lastObs.copyFrom(*oa->observation);

  }

}

#include <time.h>
#include <sys/times.h>

void testArduinoCompat() {
  printf("== TEST ARDUINO COMPAT ==\n");

  printf("- Test timing functions\n");
  unsigned long m = millis();
  delay(100);
  unsigned long diff = millis() - m;
  Q_ASSERT_ERROR_MESSAGE(90 < abs(diff)  && abs(diff) < 110, "millis() and delay() work: orig = %d, diff = %d", m, diff);

  unsigned long mic = micros();
  delayMicroseconds(1000);
  unsigned long diffMic = micros() - mic;
  Q_ASSERT_ERROR_MESSAGE(800 < abs(diffMic)  && abs(diffMic) < 1200, "micros() and delayMicroseconds() work");
  printf("-> PASSED\n");

  printf("- Test String\n");
  String a = "A";
  String b(10);
  Q_ASSERT_ERROR(a == "A");
  Q_ASSERT_ERROR(b == "10");
  a += b;
  Q_ASSERT_ERROR(a == "A10");

  String dec(100, DEC);
  String hex(0xff, HEX);
  String oct(7, OCT);
  String bin(0xff, BIN);
  Q_ASSERT_ERROR(dec == "100");
  Q_ASSERT_ERROR(hex == "ff");
  Q_ASSERT_ERROR(oct == "7");
  Q_ASSERT_ERROR(bin == "11111111");

  String c = "";
  c.concat(0x0);
  Q_ASSERT_ERROR(c == "0");
  c.concat((int)100);
  Q_ASSERT_ERROR(c == "0100");
  c.concat((unsigned int)100);
  Q_ASSERT_ERROR(c == "0100100");
  c.concat((long)100);
  Q_ASSERT_ERROR(c == "0100100100");
  c.concat((unsigned long)100);
  Q_ASSERT_ERROR(c == "0100100100100");

  printf("-> PASSED\n");
}

#include <qualia/plugins/bt/BehaviorTree.h>
using namespace BehaviorTree;


struct TestBTreeElem {
  float val;
  TestBTreeElem() : val(0) {}

  float getValue() const { return val; }
};

class PrintNode : public BehaviorTreeNode {
public:
  virtual BEHAVIOR_STATUS execute(void* agent) {
    TestBTreeElem* e = (TestBTreeElem*)agent;
    printf("::::: %f\n", e->val);
    return BT_SUCCESS;
  }

  /// This method will be invoked before the node is executed for the first time.
  virtual void init(void* agent) {
    TestBTreeElem* e = (TestBTreeElem*)agent;
    printf("Init: %f\n", e->val);
  }
};

class ChangeNode : public BehaviorTreeNode {
public:
  float inc;
  ChangeNode(float increment) : inc(increment) {}
  virtual BEHAVIOR_STATUS execute(void* agent) {
    TestBTreeElem* e = (TestBTreeElem*)agent;
    printf("Apply change: %f + %f =  %f\n", e->val, inc, e->val+inc);
    e->val += inc;
    e->val = max(e->val, 0.0f);
    return BT_SUCCESS;
  }

  /// This method will be invoked before the node is executed for the first time.
  virtual void init(void* agent) {
  }
};


void testBehaviorTree() {
  printf("== TEST BEHAVIOR TREE ==\n");

  /*
   * NOTE: À cause qu'on ne semble pas pouvoir déclrare
   */
  BehaviorTreeInternalNode* root = (BehaviorTreeInternalNode*)
                            BT.parallel(FAIL_ON_ALL, SUCCEED_ON_ONE)->CHILDREN(
                                Q_NEW(PrintNode)(),

                                BT.priority()->CHILDREN(

                                    BT.sequential()->CHILDREN(
                                        Q_NEW(FloatCondition<TestBTreeElem>)(&TestBTreeElem::getValue, GREATER_THAN_FP, 5.0f),

                                        BT.probability()->WEIGHTED_CHILDREN(
                                            BT.weighted(0.25, Q_NEW(ChangeNode)(-5)),
                                            BT.weighted(0.75, Q_NEW(ChangeNode)(0))
                                        )
                                    ),

                                    Q_NEW(ChangeNode)(+1)
                                )
                            );

 // Q_ASSERT_ERROR_MESSAGE( root->nChildren == 2, "Should have 3 children but has: %d.", (int)root->nChildren);

  TestBTreeElem elem;
  printf("Init ----------\n");
  root->init(&elem);
  for (int i=0; i<10; i++)
  {
    printf("Step # %d ----------\n", i);
    root->execute(&elem);
    printf("\n");
  }

  Q_DELETE(root);
  printf("-> PASSED\n");

}

int main() {
//  testActions();
//  testObservations();
//  testPolicies();
//  testLearning();
//  testData();
//  testBits();
//  testBinaryChromosomes();
//  testDataSet();
//  testArduinoCompat();
  testBehaviorTree();
}
