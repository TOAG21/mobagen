#include "AlignmentRule.h"
#include "../gameobjects/Boid.h"

Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f averageVelocity = boid->getVelocity();

  for (int i = 0; i < neighborhood.size(); i++) {
    averageVelocity += neighborhood[i]->getVelocity();
  }

  averageVelocity /= neighborhood.size() + 1;

  return averageVelocity;
}