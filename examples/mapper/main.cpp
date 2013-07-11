#include <qualia/core/common.h>
#include <qualia/computer/CmdLine.h>
#include <qualia/plugins/mapper/MapperBasicEnvironment.h>

#include <qualia/core/Qualia.h>
#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEDecreasingPolicy.h>
#include <qualia/rl/QLearningSoftmaxPolicy.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/core/FileExportEnvironment.h>

#include "DummyAgent.h"

#if ! is_computer()
#error "This program only works on computer platforms. Please recompile using platform=computer option."
#endif

enum AgentType {
  DUMMY,
  RL,
  BT
};

bool stopTraining = true; // uninitialized

static void stop(int);

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
  cmd.addBCmdOption("-autoconnect", &autoConnect, true, "Autoconnect the libmapper signals", false);
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
  for (int i=0; i<dimActions-1; i++, k++) {
    Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d,%s", &nActions[k], tmp) > 0, "Malformed argument <n_actions>: %s", stringNActions);
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
    agentType = BT;
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
    case BT:
    default:
      Q_WARNING("BT agent not implemented yet.");
      break;
  }

  Q_MESSAGE("--- Creating environment ---\n");
  MapperConnector connector(deviceName, peerDeviceName, autoConnect, port);
  Environment* env;
  MapperEnvironment* mapperEnv = new MapperBasicEnvironment(dimObservations, actionProperties.dim(), &connector);
  if (exportData) {
    // Export data using a FileExportEnvironment.
    char fileName[1000];
    sprintf(fileName, "export-%d.raw", agentId);
    DiskXFile* f = new(Alloc::instance()) DiskXFile(fileName, "w+");
    env = new FileExportEnvironment(mapperEnv, f, dimObservations, actionProperties.dim());
  } else
    env = mapperEnv;

  Qualia* qualia = new RLQualia(agent, env);

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
      totalReward += ((RLObservation*)oa->observation)->reward;
      nSteps++;
    }
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
    case BT:
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
