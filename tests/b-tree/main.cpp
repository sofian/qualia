/*
 * main.cpp
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

//#include "cpp_include.h"

#include <qualia/core/common.h>

#if ! is_computer()
#error "This program only works on computer platforms. Please recompile using platform=computer option."
#endif

#include <vector>

#include <qualia/computer/CmdLine.h>
#include <qualia/core/FileExportEnvironment.h>

#include <qualia/plugins/bt/BehaviorTree.h>

#include <qualia/core/Qualia.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/plugins/osc/OscRLEnvironment.h>
#include <qualia/plugins/osc/OscBasicAgent.h>

#include <cstdio>
#include <cstring>
#include <csignal>
#include <ctime>

bool stopLoop = false;

static void stop(int);

using namespace BehaviorTree;

#define ATTRACTION_MODE 1

#if ATTRACTION_MODE
#define OBSERVATION_DIM 5
#define ACTION_DIM 1
const unsigned int N_ACTIONS[] = { 2 };

#else
#define OBSERVATION_DIM 11
#define ACTION_DIM 2
const unsigned int N_ACTIONS[] = { 3, 3 };
#endif

#if ATTRACTION_MODE
enum ObservationIndices {
  X,
  Y,
  SIZE,
  HAS_CLOSEST,
  DIST_CLOSEST,
};
#else
enum ObservationIndices {
  X,
  Y,
  VX,
  VY,
  SIZE,
  HAS_CLOSEST,
  DIST_CLOSEST,
  INFLUENCE_EAST,
  INFLUENCE_NORTH,
  INFLUENCE_WEST,
  INFLUENCE_SOUTH
};
#endif

class TestBTreeAgent : public Agent {
public:
  Action* currentAction;
  const Observation* currentObservation;
  BehaviorTreeInternalNode* root;

  TestBTreeAgent(ActionProperties* actionProperties, BehaviorTreeInternalNode* root);
  virtual ~TestBTreeAgent();

  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);

  bool hasClosest() const { return currentObservation->observations[HAS_CLOSEST]; }
};

class ChooseAction : public BehaviorTreeNode
{
public:
  Action action;
  ChooseAction(ActionProperties* props, const action_dim_t* actions);
  BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
  void init(void* agent){}
};

#if ATTRACTION_MODE
const action_dim_t ATTRACT = { 1 };
const action_dim_t REPEL   = { 0 };
#else
const action_dim_t UP[]    = { 1, 2 };
const action_dim_t LEFT[]  = { 0, 1 };
const action_dim_t DOWN[]  = { 1, 0 };
const action_dim_t RIGHT[] = { 2, 1 };
#endif

int main(int argc, char** argv) {
  signal(SIGINT, stop);
  signal(SIGTERM, stop);

  int dimObservations;
  int dimActions;
  char* stringNActions;

  int oscPort;
  int oscRemotePort;
  char* oscIP;
  int agentId;

  bool exportData;
  int seed;


  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info("Simple B-Tree agent");

  cmd.addText("\nArguments:");
  cmd.addICmdArg("agent_id", &agentId, "the id of the agent", true);
  cmd.addICmdArg("dim_observations", &dimObservations, "observation dimension (without the reward)", true);
  cmd.addICmdArg("dim_actions", &dimActions, "action dimension", true);
  cmd.addSCmdArg("n_actions", &stringNActions, "number of actions as comma-separated values", true);

  cmd.addText("\nOSC Options:");
  cmd.addSCmdOption("-ip", &oscIP, "127.0.0.1", "the osc IP address", false);
  cmd.addICmdOption("-port", &oscPort, 11000, "the osc local STARTING port (actual port will be port + agent_id)", false);
  cmd.addICmdOption("-rport", &oscRemotePort, 12000, "the osc remote local port", false);

  cmd.addText("\nMisc Options:");
  cmd.addBCmdOption("-export-data", &exportData, false, "export the data to files", false );
  cmd.addICmdOption("-seed", &seed, -1, "the random seed (-1 = based on time)");

  // Read the command line
  //int mode = cmd.read(argc, argv);
  cmd.read(argc, argv);

  // Parse n actions.
  printf("Parsing actions: ");
  unsigned int nActions[100];
  char tmp[1000];
  strcpy(tmp, stringNActions);
  int k=0;
  for (int i=0; i<dimActions-1; i++, k++) {
    ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d,%s", &nActions[k], tmp) > 0, "Malformed argument <n_actions>: %s", stringNActions);
    printf("%d ", nActions[k]);
  }
  ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d", &nActions[k]), "Malformed argument <n_actions>: %s", stringNActions);
  printf("%d \n", nActions[k]);

  oscPort += agentId;
  char oscPortStr[100];
  char oscRemotePortStr[100];
  sprintf(oscPortStr, "%d", oscPort);
  sprintf(oscRemotePortStr, "%d", oscRemotePort);
  printf("Opening OSC connection (ip=%s, port=%s, remote_port=%s)\n", oscIP, oscPortStr, oscRemotePortStr);
  OscManager::initOsc(oscIP, oscPortStr, oscRemotePortStr);

  // Set random seed.
  if (seed == -1)
  {
    randomSeed(time(NULL) + agentId);
  }
  else
  {
    randomSeed(seed);
  }

  ActionProperties actionProperties(ACTION_DIM, N_ACTIONS);

#if ATTRACTION_MODE
  BehaviorTreeInternalNode* root = new SequentialNode();
  int nRepeat = (agentId + 1) * 10;
  root->addChild((new RepeatNode(nRepeat))
        ->addChild((new BoolCondition<TestBTreeAgent>))

        ->addChild((new ChooseAction(&actionProperties, UP))))
      ->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, RIGHT))))
      ->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, DOWN))))
      ->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, LEFT))));

#else
  int nRepeat = (agentId + 1) * 10;
  BehaviorTreeInternalNode* root = new SequentialNode();
  root->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, UP))))
      ->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, RIGHT))))
      ->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, DOWN))))
      ->addChild((new RepeatNode(nRepeat))
        ->addChild((new ChooseAction(&actionProperties, LEFT))));
#endif

  Agent* agent = new TestBTreeAgent(&actionProperties, root);

  printf("--- Creating environment ---\n");
  Environment* env;
  Environment* oscEnv = new OscRLEnvironment(agentId, dimObservations, actionProperties.dim());
  if (exportData) {
    char fileName[1000];
    sprintf(fileName, "export-%d.raw", agentId);
    DiskXFile* f = new(Alloc::instance()) DiskXFile(fileName, "w+");
    env = new FileExportEnvironment(oscEnv, f, dimObservations, actionProperties.dim());
  } else
    env = oscEnv;

  Qualia* qualia = new RLQualia(agent, env);

  printf("--- Initialising ---\n");
  qualia->init();

  printf("--- Starting ---\n");
  qualia->start();

  printf("--- Looping ---\n");

  stopLoop = false;
  while (!stopLoop) {
    qualia->step();
  }

  delete qualia;
  delete agent;
  delete env;
  delete root;

  return 0;
}

void stop(int sig) {
  if (stopLoop)
    exit(sig);
  else
    stopLoop = true;
}


TestBTreeAgent::TestBTreeAgent(ActionProperties* actionProperties, BehaviorTreeInternalNode* root_)
  : currentAction(0), currentObservation(0), root(root_) {
  currentAction = new Action(actionProperties);
}

TestBTreeAgent::~TestBTreeAgent() {
  delete currentAction;
}

void TestBTreeAgent::init() {
  root->init(this);
}

Action* TestBTreeAgent::start(const Observation* observation) {
  return step(observation);
}

Action* TestBTreeAgent::step(const Observation* observation) {
  currentObservation = observation;

  root->execute(this);

  return currentAction;
}


ChooseAction::ChooseAction(ActionProperties* props, const action_dim_t* actions) : action(props) {
  for (int i=0; i<props->dim(); i++)
    action[i] = actions[i];
}

BehaviorTree::BEHAVIOR_STATUS ChooseAction::execute(void* agent) {
  ((TestBTreeAgent*)agent)->currentAction->copyFrom(action);
  return BT_SUCCESS;
}
