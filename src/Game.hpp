#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <cstdint>

class Game {
private:
  bool m_isRunning;
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;

public:
  uint16_t m_windowWidth;
  uint16_t m_windowHeight;

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