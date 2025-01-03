#include "MovementSystem.hpp"

MovementSystem::MovementSystem() {
  // TODO:
  //  RequireCompoenent<TransformCompoenent>();
  //  RequireCompoenent<VelocityComponent>();
}

void MovementSystem::Update() {
  // TODO:
  // Loop all entities that the system is interested in.
  for (auto entity : GetEntities()) {
    // Update entity position based on its velocity.
  }
}
