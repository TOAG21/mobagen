#ifndef EMERGENTLEADERRULE_H
#define EMERGENTLEADERRULE_H

#include "FlockingRule.h"

class Boid;

/* Steer to move toward center of mass of local flockmates */
class EmergentLeaderRule : public FlockingRule {
public:
  explicit EmergentLeaderRule(World* pWorld, float weight = 1., bool isEnabled = true) : FlockingRule(pWorld, Color::NavajoWhite, weight, isEnabled) {}

  std::unique_ptr<FlockingRule> clone() override {
    // Créer un pointeur concret en utilisant le constructeur abstrait parent
    return std::make_unique<EmergentLeaderRule>(*this);
  }

  const char* getRuleName() override { return "Emergent Leader Rule"; }

  const char* getRuleExplanation() override { return "Follow Movements of a defined leader"; }

  virtual float getBaseWeightMultiplier() override { return 1.; }

  Vector2f computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) override;
};

#endif