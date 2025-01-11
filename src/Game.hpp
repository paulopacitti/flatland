#ifndef GAME_HPP
#define GAME_HPP

#include "ECS.hpp"
#include <SDL2/SDL.h>
#include <cstdint>

const int FPS = 60;
const int MS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool m_isRunning;
  uint64_t m_previousFrameTime;
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;

  void setup();
  double getDeltaTime();

  Registry *m_registry;

public:
  uint16_t windowWidth;
  uint16_t windowHeight;

  Game();
  ~Game();
  void initialize();
  void run();
  void processInput();
  void update();
  void render();
  void destroy();
};

#endif