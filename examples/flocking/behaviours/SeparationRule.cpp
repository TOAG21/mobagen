#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f separatingForce = Vector2f::zero();
  float maxForce = 500.0f;

  Vector2f seperationVector, calcHolder;
  for (int i = 0; i < neighborhood.size(); i++) {
    calcHolder = boid->getPosition() - neighborhood[i]->getPosition();
    if (calcHolder.getMagnitude() < desiredMinimalDistance)
    {
      seperationVector += calcHolder;
    }
  }

  if (neighborhood.size() < 1)
  {
    return separatingForce;
  }

  if (seperationVector.getMagnitude() > 0)
  {
    separatingForce = seperationVector * (desiredMinimalDistance / (seperationVector.getMagnitude() / desiredMinimalDistance));
    //separatingForce = seperationVector * (desiredMinimalDistance / seperationVector.getMagnitude());
  }

  if (separatingForce.sqrMagnitude() > (maxForce * maxForce))
  {
    separatingForce = separatingForce.normalized() * maxForce;
  }

  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valusHasChanged = false;

  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valusHasChanged = true;
  }

  return valusHasChanged;
}
