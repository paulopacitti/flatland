#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../Component.hpp"
#include "../ECS.hpp"
#include <SDL2/SDL.h>

class RenderSystem : public System {
  public:
    RenderSystem() {
      requireComponent<TransformComponent>();
      requireComponent<SpriteComponent>();
    }

    void update(SDL_Renderer* renderer) {
      for (auto entity : getEntities()) {
        const auto transform = entity.getComponent<TransformComponent>();
        const auto sprite = entity.getComponent<SpriteComponent>();

        // TODO: draw the textures of each sprite
        SDL_Rect obj = {static_cast<int>(transform.position.x),
                        static_cast<int>(transform.position.y), sprite.width,
                        sprite.height};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &obj);
      }
    }
};

#endif
