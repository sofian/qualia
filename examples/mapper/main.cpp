#include <qualia/core/common.h>
#include <qualia/computer/CmdLine.h>
#include <qualia/plugins/mapper/MapperBasicEnvironment.h>
#include <qualia/plugins/mapper/MapperRLEnvironment.h>
#include <qualia/plugins/bt/BehaviorTree.h>

#include <qualia/core/Qualia.h>
#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEDecreasingPolicy.h>
#include <qualia/rl/QLearningSoftmaxPolicy.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/core/FileExportEnvironment.h>

#include "DummyAgent.h"
#include "SimpleBehaviorTreeAgent.h"
#include "InfluenceRemappingEnvironment.h"

#if ! is_computer()
#error "This program only works on computer platforms. Please recompile using platform=computer option."
#endif

enum AgentType {
  DUMMY,
  RL,
  BTREE
};

bool stopTraining = true;

static void stop(int);

using namespace BehaviorTree;

int main(int argc, char** argv) {
  signal(SIGINT, stop);
  signal(SIGTERM, stop);

  char* agentTypeStr;
  AgentType agentType;

  int nHidden;
  float learningRate;
  float learningRateDecay;
  float weightDecay;
  float epsilon;
  float epsilonDecay;
  bool useSoftmax;
  float temperature;
  float lambda;
  float gamma;
  int dimObservations;
  int dimActions;
  char* stringNActions;

  char* deviceName;
  char* peerDeviceName;
  bool autoConnect;
  int port;

  int agentId;

  bool exportData;
  int outputEvery;
  int seed;

  char* saveModelFileName;
  char* loadModelFileName;
  bool isLearning;

//  bool isRemoteAgent;

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info("Libmapper Agent");

  cmd.addText("\nArguments:");
  cmd.addICmdArg("agent_id", &agentId, "the id of the agent", true);
  cmd.addICmdArg("dim_observations", &dimObservations, "observation dimension (without the reward)", true);
  cmd.addICmdArg("dim_actions", &dimActions, "action dimension", true);
  cmd.addSCmdArg("n_actions", &stringNActions, "number of actions as comma-separated values", true);

  cmd.addText("\nLibmapper Options:");
  cmd.addSCmdOption("-device", &deviceName, "agent", "the device name (without ordinal)", false);
  cmd.addSCmdOption("-peer-device", &peerDeviceName, "test.1", "the peer device name (with ordinal)", false);
  cmd.addBCmdOption("-autoconnect", &autoConnect, false, "Autoconnect the libmapper signals", false);
  cmd.addICmdOption("-port", &port, 9000, "Initial mapper port", false);

  cmd.addText("\nAgent Options:");
  cmd.addSCmdOption("-agent-type", &agentTypeStr, "dummy", "the agent type (options are: dummy, rl (reinforcement learning), bt (behavior tree))", false);

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-nhu", &nHidden, 5, "number of hidden units", true);

  cmd.addText("\nLearning Options:");
//  cmd.addICmdOption("-iter", &max_iter, 25, "max number of iterations");
  cmd.addRCmdOption("-lr", &learningRate, 0.01, "learning rate", true);
  cmd.addRCmdOption("-lrd", &learningRateDecay, 0.001, "learning rate decay", true);
  cmd.addRCmdOption("-wd", &weightDecay, 0, "weight decay", true);

  cmd.addText("\nReinforcement Learning Options:");
  cmd.addRCmdOption("-gamma", &gamma, 0.999, "the gamma value", true);
  cmd.addRCmdOption("-lambda", &lambda, 0.1, "the lambda value of the TD-lambda algorithm", true);

  cmd.addText("\nPolicy Options:");
  cmd.addRCmdOption("-e", &epsilon, 0.1, "epsilon value", true);
  cmd.addRCmdOption("-ed", &epsilonDecay, 0, "epsilon decay", true);
  cmd.addBCmdOption("-softmax", &useSoftmax, false, "use softmax policy", true);
  cmd.addRCmdOption("-temp", &temperature, 1.0f, "softmax temperature param", true);

  cmd.addText("\nMisc Options:");
  cmd.addBCmdOption("-export-data", &exportData, false, "export the data to files", false );
  cmd.addBCmdOption("-no-learning", &isLearning, true, "don't learn (just apply policy)", false );
//  cmd.addBCmdOption("-remote-agent", &isRemoteAgent, false, "remote controlled agent (ie. actions sent by OSC)", false );
  cmd.addICmdOption("-every", &outputEvery, 100, "output mean reward every X steps", false );
  cmd.addICmdOption("-seed", &seed, -1, "the random seed (-1 = based on time)");
//  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
//  cmd.addICmdOption("-load_valid", &max_load_valid, -1, "max number of examples to load for valid");
//  cmd.addSCmdOption("-valid", &valid_file, "", "validation file, if you want it");
//  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addSCmdOption("-load", &loadModelFileName, "", "the model file to load from");
  cmd.addSCmdOption("-save", &saveModelFileName, "", "the model file to save to");
//  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  //int mode = cmd.read(argc, argv);
  cmd.read(argc, argv);

  // Parse n actions.
  Q_MESSAGE("Parsing actions: ");
  unsigned int nActions[100];
  char tmp[1000];
  strcpy(tmp, stringNActions);
  int k=0;
  int nActions0 = -1;
  for (int i=0; i<dimActions-1; i++, k++) {
    Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d,%s", &nActions[k], tmp) > 0, "Malformed argument <n_actions>: %s", stringNActions);
    if (nActions0 == -1)
      nActions0 = nActions[k];
    else
      Q_ASSERT_ERROR_MESSAGE( nActions0 == nActions[k], "Invalid n. action string: %s. Qualia's mapper plugin only supports actions with the name number of actions component per dimension.");
  }
  Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d", &nActions[k]), "Malformed argument <n_actions>: %s", stringNActions);

  // Set random seed.
  randomSeed(seed == -1 ? time(NULL) + agentId : seed);

  if (!isLearning)
    Q_MESSAGE("Learning switched off\n");

  // Parse agent type.
  if (strcmp(agentTypeStr, "dummy") == 0)
    agentType = DUMMY;
  else if (strcmp(agentTypeStr, "rl") == 0)
    agentType = RL;
  else if (strcmp(agentTypeStr, "bt") == 0)
    agentType = BTREE;
  else
    Q_ERROR("Unsupported -agent-type parameter: %s.", agentType);

  Q_MESSAGE("--- Creating agent ---\n");
  ActionProperties actionProperties(dimActions, nActions);

  Agent* agent;
  switch (agentType) {
    case DUMMY:
      agent = new DummyAgent(&actionProperties);
      break;
    case RL: {
        // Agent is a Q-Learning agent.
        NeuralNetwork* net = new NeuralNetwork(
            dimObservations + actionProperties.dim(), nHidden, 1, learningRate,
            learningRateDecay, weightDecay, false);
        QFunction* qFunc = new QFunction(net, dimObservations, &actionProperties);
        Policy* policy = (
            useSoftmax ? (Policy*) new QLearningSoftmaxPolicy(temperature,
                epsilon) :
                (Policy*) new QLearningEDecreasingPolicy(epsilon, epsilonDecay));
        QLearningAgent* qAgent = new QLearningAgent(qFunc, policy, dimObservations,
                                                    &actionProperties, lambda, gamma);
        qAgent->isLearning = isLearning;
        agent = qAgent;
      }
      break;
    case BTREE: {
      // Here we will presuppose that we have actions and obs. of dim. 2.
        Q_ASSERT_WARNING_MESSAGE(dimActions == 2 && dimObservations == 2, "Using the behavior tree supposes that dim_actions = 2 and dim_observations = 2.");

        action_dim_t idle[] = { nActions[0]/2, nActions[1] / 2  };

        BehaviorTreeNode* root =

            BT.priority(CONTINUE)->setChildren(

              BT.sequential()->setChildren(
                Q_NEW(FloatCondition<SimpleBehaviorTreeAgent>)(&SimpleBehaviorTreeAgent::getEnergy, LESS_THAN_FP, 12.0),
                Q_NEW(FunctionCall<SimpleBehaviorTreeAgent>(&SimpleBehaviorTreeAgent::debug)),
                BT.probability()->setWeightedChildren(
                  BT.weighted(0.5, Q_NEW(FunctionCall<SimpleBehaviorTreeAgent>(&SimpleBehaviorTreeAgent::moveTowardsInfluence))),
                  BT.weighted(0.5, Q_NEW(RandomAction)(&actionProperties)),
                  BT_END_WEIGHTED),
//                Q_NEW(RandomAction)(&actionProperties),
                Q_NEW(FunctionCall<SimpleBehaviorTreeAgent>(&SimpleBehaviorTreeAgent::accumulateEnergy)),
                BT_END),

              BT.sequential()->setChildren(
                Q_NEW(FunctionCall<SimpleBehaviorTreeAgent>(&SimpleBehaviorTreeAgent::resetEnergy)),
                BT.repeat(1000)->setChild(
                  BT.sequential()->setChildren(
                    Q_NEW(FloatCondition<SimpleBehaviorTreeAgent>)(&SimpleBehaviorTreeAgent::getInfluenceNorm, LESS_THAN_FP, 1.0),
                    BT.probability()->setWeightedChildren(
                      BT.weighted(0.5, Q_NEW(FunctionCall<SimpleBehaviorTreeAgent>(&SimpleBehaviorTreeAgent::moveAwayFromInfluence))),
                      BT.weighted(0.5, Q_NEW(RandomAction)(&actionProperties)),
                      BT_END_WEIGHTED),
                    BT_END)),
                BT_END),

              BT_END);

        agent = new SimpleBehaviorTreeAgent(agentId, &actionProperties, root);

//            BT_PARALLEL(FAIL_ON_ALL, SUCCEED_ON_ONE)->CHILDREN(
//                                   BT_SEQUENTIAL()->CHILDREN(
//                                     Q_NEW(FloatCondition<SimpleBehaviorTreeAgent>)(&SimpleBehaviorTreeAgent::getInfluenceX, GREATER_THAN_FP, 0.0),
//                                     Q_NEW(SetActionDim)
//                                   )
//
//            );
      }
    default:
      Q_WARNING("BT agent not implemented yet.");
      break;
  }

  Q_MESSAGE("--- Creating environment ---\n");
  MapperConnector connector(deviceName, peerDeviceName, autoConnect, port);
  Environment* env;
  MapperEnvironment* mapperEnv;
  if (agentType == RL)
    mapperEnv = new MapperRLEnvironment(dimObservations, &actionProperties, &connector);
  else
    mapperEnv = new MapperBasicEnvironment(dimObservations, &actionProperties, &connector);

  if (exportData) {
    // Export data using a FileExportEnvironment.
    char fileName[1000];
    sprintf(fileName, "export-%d.raw", agentId);
    DiskXFile* f = Q_NEW(DiskXFile)(fileName, "w+");
    env = new FileExportEnvironment(mapperEnv, f, dimObservations, actionProperties.dim());
  } else
    env = mapperEnv;

  InfluenceRemappingEnvironment infEnv(env);

  Qualia* qualia = new Qualia(agent, &infEnv);

  Q_MESSAGE("--- Initialising ---\n");
  qualia->init();

  // Check if load needed.
  if (strcmp(loadModelFileName, "") != 0) {
    DiskXFile loadFile(loadModelFileName, "r");
    ((QLearningAgent*)agent)->qFunction->load(&loadFile);
  }

  Q_MESSAGE("--- Starting ---\n");
  qualia->start();

  stopTraining = false;
  Q_MESSAGE("--- Looping ---\n");
  while (!stopTraining) {
    unsigned long nSteps = 0;
    float totalReward = 0;
    for (int i=0; i<outputEvery; i++) {
      ObservationAction* oa = qualia->step();
      if (agentType == RL)
        totalReward += ((RLObservation*)oa->observation)->reward;
      nSteps++;
    }
    if (agentType == RL)
      Q_MESSAGE("Mean reward (agent #%d): %f\n", agentId, (double) totalReward / nSteps);
  }

  // Check if load needed.
  if (strcmp(saveModelFileName, "") != 0) {
    Q_MESSAGE("--- Saving (NOT IMPLEMENTED FOR NOW) ---\n");
  }

  Q_MESSAGE("--- Cleaning up ---\n");

  switch (agentType) {
    case DUMMY:
      delete agent;
      break;
    case RL: {
        QLearningAgent* qAgent = (QLearningAgent*)agent;
        delete qAgent->qFunction->function;
        delete qAgent->qFunction;
        delete qAgent->policy;
        delete qAgent;
      }
      break;
    case BTREE:
      delete agent;
      break;
    default:
      Q_WARNING("BT agent not implemented yet.");
      break;
  }

  delete mapperEnv;
  if (exportData)
    delete env;

  delete qualia;

  return 0;
}

void stop(int sig) {
  if (stopTraining)
    exit(sig);
  else
    stopTraining = true;
}

