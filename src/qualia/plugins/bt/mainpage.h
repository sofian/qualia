/**
\mainpage Libbehavior Documentation

\section intro Introduction to the library

This is the API Documentation for Libbehavior, a reactive AI library based on the concept of "behavior trees". A behavior tree is a way of organizing a collection of states and the decision processes for when to move between them. Behavior trees have many similiarities to Finite State Machines, but they have a number of additional features that make them more suitable for representing complex and potentially parallel behaviors.

Like Finite State Machines and unlike planners, Behavior Trees are reactive. This means that they do not look ahead to the future, but simply make decisions based on their current state and current inputs. This has a number of implications. First, it is clear that Behavior Trees should not be used for solving intense algorithmic problems like playing chess or pathfinding. Instead, Behavior Trees excel at things like making interesting enemies and non-player characters in RPG, shooting, or adventure games. They can also provide a convenient scripting language for implementing simple, reusable chunks of logic, in cases where the full power (and full complexity) of C++ is not needed.

If this sounds useful to you, continue to the next section, where I explain exactly what behavior trees are and how they work.

\section whatis What is a Behavior Tree?

A Behavior Tree, as one would guess, is a branching data structure that is used to represent behaviors. There are two types of nodes in a behavior tree: internal nodes and leaf nodes. Leaf nodes represent the most primitive actions that could be taken by a character. In a game, this could be walking in a particular direction, using a particular item, and doing a particular attack. The behavior of an agent can be defined as a sequence of these primitive actions, with the caveat that some of the actions could be happening simultaneously.

Of course, a fixed linear sequence of actions is not a very interesting behavior. It lacks awareness of the situation and adaptivity. These qualities are given to the agent via the internal nodes. These nodes decide which of their children nodes to execute, using a number of different schemes. One internal node, the Parallel Node, executes all of its children simultaneously. By combining the Parallel Node with other nodes like the Priority or Probability Nodes, interesting behaviors can be developed that show awareness and adaptivity.

Each node in a behavior tree can keep its own state. For example, this could be the current position of executing for a sequential node. In addition, when a behavior tree is executed, external state can be passed in to the structure through the void* "agent" argument. Each node is obligated to return a status after it has finished executing. The available statuses are "Success", "Failure", and "Running". Every timeslice (where a timeslice can be a particular number of milliseconds, a turn in a strategy game, etc.) the root node of the behavior tree is executed. The root node, in turn, selects some number of its children and executes them. This continues down the tree until execution reaches the leaf nodes. The leaf nodes update their state and select a status. The status information is interpreted at each level of the tree. At the top level, the root node selects a status and returns it. (TODO: insert diagram)

Generally, it is considered desireable to limit a node to either keeping internal state or using external state, but not both. This reduces interdependence, which simplifies debugging and promotes reuse. Often, nodes that keep internal state are also internal nodes, and nodes that use external state are leaf nodes.

\section internal What Internal Nodes are Available?

This is a brief behavioral description of each of the supplied internal nodes. For specifics, please consult each node's documentation page.

\subsection countlimit Count Limit Node
This node restricts a particular path of behavior from executing more than the specified number of times. It can either put a strict limitation in repetition, or reset the limitation after some time. This is useful for, e.g. preventing your agents from repeating the same line of dialog to the point where the player will become annoyed.
\sa BehaviorTree::CountLimitNode
\subsection parallel ParallelNode
This is a powerful node that executes all of its children at the same time. This not just useful for simultanous behaviors, it can also be used for pursuing a main path of behavior while simultanously monitoring the world for problems that would make the behavior invalid. Examples of this type of usage are given in the test bench.
\sa BehaviorTree::ParallelNode
\subsection priority Priority Node
The Priority Node maintains an ordered sequence of behaviors, where earlier ones are more desireable (but may fail to complete), and later ones are less desireable, but presumably more likely to work. For example, if a character is trying to get through a door and they have the key in pocket, they can simply take out the key and open the door. If they don't have a key in pocket, that behavior will fail. The next behavior in priority order may be to go to the backyard and get the spare key. If that fails, the next behavior may be to call someone who has a key. This will continue until either one of the approaches succeeds, in which case the priority node will return success, or all of the nodes fail. In the latter case, the priority node will also return failure.
\sa BehaviorTree::PriorityNode
\subsection probability Probability Node
The probability node introduces randomness into the agent's behavior. It works by associating a weight with each of its children. When it needs to select a behavior, it makes a random choice among its children, based on their weights. The weighting approach facilitates "tweaking" behaviors by allowing changes to be made without requiring that all the probabilities be re-balanced.
\sa BehaviorTree::ProbabilityNode
\subsection repeat Repeat Node
The Repeat Node is unusual because it only has one child. It runs that child either a fixed number of times or indefinitely.
\sa BehaviorTree::RepeatNode
\subsection sequential Sequential Node
The Sequential Node is the workhorse internal node. It simply runs all of its children in sequential order. If one of them fails, it also fails. Once all of them succeeds, it also succeeds.
\sa BehaviorTree::SequentialNode

By composing these internal nodes together or possibly making your own, a large range of behavior can be expressed in a compact and reusable way.

\section leaf What Leaf Nodes are Available?

Since leaf nodes are application-specific, you will have to implement most of them yourself. However, there are a handful of useful nodes provided. Some of them can wrap function and member calls in order to save you time and reduce code duplication. Others are handy "stand ins" for when some behavior is not yet implemented, or when you are debugging behaviors.

\subsection always AlwaysRunning, AlwaysSuccess, AlwaysFailure
These are the simplest leaf nodes. They simply return the indicated status value.
\sa BehaviorTree::AlwaysRunning\n
	BehaviorTree::AlwaysSuccess\n
	BehaviorTree::AlwaysFailure\n

\subsection after SuccessAfter, FailureAfter
These have the 'running' status for a given number of iterations, then they return the indicated status.
\sa BehaviorTree::SuccessAfter\n
    BehaviorTree::FailureAfter\n

\subsection condition BoolCondition, FloatCondition, IntCondition
These wrap function calls (with no arguments) that return the specified type. The BoolCondition can check whether the return value is a given one. The FloatCondition and IntCondition have a full set of comparisons (==,<,<=,>,>=).
\sa BehaviorTree::BoolCondition\n
	BehaviorTree::FloatCondition\n
	BehaviorTree::IntCondition\n

\subsection functionCall FunctionCall
This wraps a function call with a void* argument that has no return value.
\sa BehaviorTree::FunctionCall\n

\section Behavior Tree Examples

Included with the library are several examples of behavior trees in action. They are implemented in the context a "shoot-em-up" game, but of course the principles they use are applicable to anything. This is a short description of each of the behavior examples. For more information, consult the relevant "ScenarioX.h" header file. Before doing that, try actually playing the scenario to understand the behavior it implements.

\section scenario1 Scenario 1

This scenario implements to following behavior:

-# Try to line up a shot on the player.
-# If you have a shot lined up, fire, and then wait 500ms for the weapon to "recharge".

This is the code that creates the behavior:

\code
opponent->brain
	->addChild((new RepeatNode(-1))
		->addChild((new SequentialNode())
			->addChild(new BoolCondition<GameObject>(&GameObject::alignedWithPlayer,true))
			->addChild(new Fire())
			->addChild(new Cooldown(500))))
	->addChild(new TurnTowardsTarget(.5));
\endcode 

Note that the "brain" node is a parallel node by default. The first indented section implements the firing behavior. The firing sequence is composed of

-# A check to make sure there is a shot lined up.
-# Actually firing at the player.
-# A 500ms cooldown period.

That firing sequence is wrapped in a RepeatNode, because the enemy should fire continuously at the player. If the RepeatNode was not there, the enemy would only fire once at the player, since after it fired once, that branch of the parallel node would have been considered a "success".

The second parallel node branch demonstrates that leaf nodes do not have to be the simplest behaviors. "Turn towards a target" is a single unit of behavior, but the implementation is not as simple as, say, turn left or right. It would have been possible to implement TurnTowardsTarget using a behavior tree with "turn left" and "turn right" as leaves, but the benefits of doing so are questionable.

Note the indentation style used for this section of code. This style of "chaining" function calls is the preferred way of creating behavior trees, because it concisely expresses the structure of the code.

\section scenario2 Scenario 2

This scenario demonstrates how behaviors can be reused within a project. The scenario implements an enemy that fires projectiles that "home in" on the player. However, this enemy has a special feature. If its health is below a critical threshold, the enemy will attempt a suicidal ramming attack on the player. The switch between the "firing" and "ramming" behavior is controlled with a priority node. The behavior tree used for the ramming attack is exactly the same as the behavior tree controlling the behavior of the projectiles. This shows that even if two objects have different types, as long as the behavior tree is written to the appropriate level of abstraction, it can apply across types.

In this case, the behavior reuse is done by using a function that returns the appropriate Node. This approach could potentially be extended in more complex projects to use function arguments to customize the resulting behavior, lookup tables, or other features.

\section scenario3 Scenario 3

Scenario 3 demonstrates a more complex behavior, and also shows how to incorporate randomness into behavior. The behavior is as follows:

-# Keep lined up with the player, and fire if aligned.
-# Occasionally switch between firing positions, to keep the player confused.

The "occasional" aspect of the position switch is controlled by a probability node. The firing behavior has a weight of 5.0, and the switching behavior has a weight of 1.0. Therefore, the enemy will decide switch positions about 1/6 of the times it is considering what to do.

\section scenario4 Scenario 4

Scenario 4 is another variant of an evasive enemy. This time, instead of switching positions randomly, the enemy bases its movement on the position of the player. If the player has a shot lined up, the enemy will move away in a random direction.

\section scenario5 Scenario 5

This scenario gives the enemy an ability that can destroy all projectiles within a certain proximity to it. However, this ability can only be activated 5 times, and it takes 2 seconds to recharge between uses. The 5 uses limitation is implemented with a count limit node.

\section scenario6 Scenario 6

Scenario 6 is a version of the classic game "Space Invaders". It demonstrates hundreds of agents running at the same time. The entire AI code takes about 10 lines.

\section scenario7 Scenario 7

Scenario 7 shows how a story can be told through the use of behavior. In this case, the player is confonted with 4 enemies that work in unison. When 3 of the enemies are destroyed, the remaining one "surrenders" to the player.

\section scenario8 Scenario 8

Scenario 8 shows a non-obvious way to use behavior trees. In addition to controlling the behavior of the agents in the world, they can also be used in a director role to control the behavior of the game itself. In this case, the game only spawns a "secret" enemy if the player defeated the previous two enemies without getting hit.

\section scenario9 Scenario 9

Scenario 9 shows how to manage synchronization and communication between agents in the behavior tree framework. Three projectiles are spawned which all wait for a common signal before beginning to move in unison. This scenario also demonstrates the usage of a FunctionCall node.

\section scenario10 Scenario 10

Scenario 10 is a further demonstration of the FloatCondition and FunctionCall nodes. An enemy ship is cloaked, causing it to appear nearly transparent. However, if the player gets close enough, they can disrupt the cloaking field, making the ship permanently visible.

*/