#ifndef WANDERRULE_H
#define WANDERRULE_H

#include "FlockingRule.h"

class Boid;

/* Steer to move toward center of mass of local flockmates */
class WanderRule : public FlockingRule {
public:
  explicit WanderRule(World* pWorld, float weight = 1., bool isEnabled = true)
      : FlockingRule(pWorld, Color::DarkGoldenrod, weight, isEnabled) {}

  std::unique_ptr<FlockingRule> clone() override {
    // Créer un pointeur concret en utilisant le constructeur abstrait parent
    return std::make_unique<WanderRule>(*this);
  }

  const char* getRuleName() override { return "Wander Rule"; }

  const char* getRuleExplanation() override { return "Make Random movements when not being lead"; }

  virtual float getBaseWeightMultiplier() override { return 1.; }

  Vector2f computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) override;
};

#endif