#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../Component.hpp"
#include "../ECS.hpp"

class MovementSystem : public System {
  public:
    MovementSystem() {
      requireComponent<TransformComponent>();
      requireComponent<RigidBodyComponent>();
    }

    void update(const double& dt) {
      for (auto entity : getEntities()) {
        auto& transform = entity.getComponent<TransformComponent>();
        const auto& rigidBodyComponent =
            entity.getComponent<RigidBodyComponent>();

        transform.position.x += rigidBodyComponent.velocity.x * dt;
        transform.position.y += rigidBodyComponent.velocity.y * dt;

        spdlog::info(
            "[MovementSystem] entityId=" + std::to_string(entity.getId()) +
            " position is now (" + std::to_string(transform.position.x) + ", " +
            std::to_string(transform.position.y) + ")");
      }
    }
};

#endif
