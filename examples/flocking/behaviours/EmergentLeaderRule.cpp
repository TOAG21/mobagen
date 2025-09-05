#include "EmergentLeaderRule.h"
#include "../gameobjects/Boid.h"

Vector2f EmergentLeaderRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f leaderForce;
  Vector2f boidAgentPos = boid->getPosition();

  if (neighborhood.size() < 1) {
    Vector2f zero(0.0f, 0.0f);
    return zero;
  }

  Vector2f cohesionCenterMass;
  for (int i = 0; i < neighborhood.size(); i++) {
    cohesionCenterMass += neighborhood[i]->getPosition();
  }
  cohesionCenterMass.x /= neighborhood.size();
  cohesionCenterMass.y /= neighborhood.size();

  // get force vector from center of mass
  leaderForce = cohesionCenterMass - boidAgentPos;
  return leaderForce;
}