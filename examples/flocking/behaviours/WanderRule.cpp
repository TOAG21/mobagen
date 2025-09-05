#include "WanderRule.h"
#include "../gameobjects/Boid.h"

Vector2f WanderRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f wanderForce = Vector2f(0.0f,0.0f);

  if (boid->hasLeader)
  {
    return wanderForce;
  }

  if (!(boid->wanderCooldown > 0.0f))
  {
    boid->wanderAngle = (float)(rand() % 320) - 160.0f;
    boid->wanderCooldown = (float)(rand() % 100) / 10.0f;
  }
  if (boid->wanderAngle > 2.0f)
  {
    wanderForce.x = boid->wanderCoeffecientC * boid->velocity.x - boid->wanderCoeffecientS * boid->velocity.y;
    wanderForce.y = boid->wanderCoeffecientS * boid->velocity.x + boid->wanderCoeffecientC * boid->velocity.y;
  } 
  else if (boid->wanderAngle < -2.0f) 
  {
    wanderForce.x = boid->wanderCoeffecientC * boid->velocity.x + boid->wanderCoeffecientS * boid->velocity.y;
    wanderForce.y = boid->wanderCoeffecientC * boid->velocity.y - boid->wanderCoeffecientS * boid->velocity.x;
  }

  return wanderForce;
}