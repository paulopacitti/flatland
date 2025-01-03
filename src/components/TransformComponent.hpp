#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <glm/glm.hpp>

struct TrasnformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  float rotation;
};

#endif