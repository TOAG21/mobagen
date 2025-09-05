#include "EmergentLeaderRule.h"
#include "../gameobjects/Boid.h"

Vector2f EmergentLeaderRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f leaderForce = Vector2f(0.0f,0.0f);
  Boid* newLeader = nullptr;
  Vector2f boidAgentPos = boid->getPosition();

  //if no boids are visible then become free
  if (neighborhood.size() < 1) {
    Vector2f zero(0.0f, 0.0f);
    boid->hasLeader = false;
    boid->leader = nullptr;
    return zero;
  }

  if (boid->isLeader)
  {
    return leaderForce;
  }

  //look for leaders by finiding boids in fornt heading in same direction
  //if that boid has a leader follow them.
  for (int i = 0; i < neighborhood.size(); i++) 
  {
      Vector2f leadingCheck = neighborhood[i]->velocity;
      if (abs(acos(boid->velocity.x / boid->velocity.getMagnitude()) - acos(leadingCheck.x / leadingCheck.getMagnitude())) < 0.524f) 
      {
        leadingCheck = neighborhood[i]->getPosition() - boidAgentPos;
        if (signbit(leadingCheck.x) == signbit(boid->velocity.x) && signbit(leadingCheck.y) == signbit(boid->velocity.y))
        {
          if (neighborhood[i]->hasLeader) 
          {
            newLeader = neighborhood[i]->leader;
            break;
          } 
          else 
          {
            newLeader = neighborhood[i];
          }
        }
      }
  }

  //if there is a leader to follow
  if (newLeader != nullptr) 
  {
    //if that leader is not our current one
    if (newLeader != boid->leader)
    {
      if (boid->hasLeader) 
      {
        boid->leader->followers--;
        // if leader has no more followers make normal
        if (boid->leader->followers == 0) 
        {
          boid->leader->isLeader = false;
        }
      }
      boid->leader = newLeader;
      boid->hasLeader = true;
      boid->leader->isLeader = true;
      boid->leader->followers++;
    }

    leaderForce = (newLeader->getPosition() - boidAgentPos + newLeader->velocity) / 3.0f;
  } 
  else //we've found no leader to follow 
  {
    if (boid->hasLeader) {
      boid->leader->followers--;
      // if leader has no more followers make normal
      if (boid->leader->followers == 0) {
        boid->leader->isLeader = false;
      }
    }
    boid->hasLeader = false;
    boid->leader = nullptr;
  }

  return leaderForce;
}