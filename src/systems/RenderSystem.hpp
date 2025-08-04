#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../AssetStore.hpp"
#include "../Component.hpp"
#include "../ECS.hpp"
#include <SDL2/SDL.h>

class RenderSystem : public System {
  public:
    RenderSystem() {
      requireComponent<TransformComponent>();
      requireComponent<SpriteComponent>();
    }

    void update(SDL_Renderer* renderer,
                std::unique_ptr<AssetStore>& assetStore) {
      for (auto entity : getEntities()) {
        const auto transform = entity.getComponent<TransformComponent>();
        const auto sprite = entity.getComponent<SpriteComponent>();

        // Set the source rectangle of our original sprite texture
        SDL_Rect srcRect = sprite.srcRect;
        SDL_Rect dstRect = {
            static_cast<int>(transform.position.x),
            static_cast<int>(transform.position.y),
            static_cast<int>(sprite.width * transform.scale.x),
            static_cast<int>(sprite.height * transform.scale.y)};
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Set the destination rectangle with the position to be rendered
        SDL_RenderCopyEx(renderer, assetStore->getTexture(sprite.assetId),
                         &srcRect, &dstRect, transform.rotation, NULL, flip);
      }
    }
};

#endif
