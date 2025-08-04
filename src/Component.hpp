#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>

struct TransformComponent {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    TransformComponent(glm::vec2 position = glm::vec2(0, 0),
                       glm::vec2 scale = glm::vec2(1, 1),
                       float rotation = 0.0) {
      this->position = position;
      this->scale = scale;
      this->rotation = rotation;
    }
};

struct RigidBodyComponent {
    glm::vec2 velocity;

    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0)) {
      this->velocity = velocity;
    }
};

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    SDL_Rect srcRect;

    SpriteComponent(std::string assetId = "", int width = 0, int height = 0,
                    int srcRectX = 0, int srcRectY = 0) {
      this->assetId = assetId;
      this->width = width;
      this->height = height;
      this->srcRect = {srcRectX, srcRectY, width, height};
    }
};

#endif
