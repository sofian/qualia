#include <qualia/core/Environment.h>
#include <qualia/util/map.h>

#define INFLUENCE_MIN (-1.070770)
#define INFLUENCE_MAX (+1.695009)

class InfluenceRemappingEnvironment : public Environment {
public:
  Environment* env;

  InfluenceRemappingEnvironment(Environment* env_) : env(env_) {}
  virtual ~InfluenceRemappingEnvironment() {}

  static Observation* remapObservation(Observation* obs) {
    for (unsigned int i=0; i<obs->dim(); i++) {
      obs->observations[i] = mapReal(obs->observations[i], 0.0, 1.0, -1.0, 1.0);
    }
    return obs;
  }

  virtual void init() {
    env->init();
  }

  /// Sends the first  observation.
  virtual Observation* start() {
    return remapObservation(env->start());
  }

   /// Performs action *action* and returns observation.
   virtual Observation* step(const Action* action) {
     printf("Stepping\n");
     return remapObservation(env->step(action));
   }
};
