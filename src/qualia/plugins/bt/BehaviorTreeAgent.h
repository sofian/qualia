/*
 * BehaviorTreeAgent.h
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef BEHAVIORTREEAGENT_H_
#define BEHAVIORTREEAGENT_H_

#include <qualia/core/Agent.h>
#include <qualia/plugins/bt/BehaviorTree.h>

class BehaviorTreeAgent : public Agent {
public:
  Action currentAction;
  const Observation* currentObservation;
  BehaviorTree::BehaviorTreeNode* root;

  BehaviorTreeAgent(ActionProperties* properties, BehaviorTree::BehaviorTreeNode* root);
  virtual ~BehaviorTreeAgent();

  /// Initializes the agent.
  virtual void init();

  /// Chooses the first action in the episode.
  virtual Action* start(const Observation* observation);

  /// Performs an episode step.
  virtual Action* step(const Observation* observation);

  /// Can be used by leaf nodes to access the current observation under scrutiny.
  const Observation* getCurrentObservation() const { return currentObservation; }

  /// Can be used by leaf nodes to access the current observation under scrutiny.
  Action& getCurrentAction() { return currentAction; }
};

#endif /* BEHAVIORTREEAGENT_H_ */
