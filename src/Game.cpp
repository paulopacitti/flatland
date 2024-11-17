#include "Game.hpp"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <iostream>

Game::Game() {
  m_isRunning = false;
  std::cout << "Game constructor called!" << std::endl;
}

Game::~Game() { std::cout << "Game destructor called!" << std::endl; }

void Game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
    return;
  }

  m_windowWidth = 800;
  m_windowHeight = 600;
  m_window = SDL_CreateWindow("flatland", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, m_windowWidth,
                              m_windowHeight, SDL_WINDOW_RESIZABLE);
  if (!m_window) {
    std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
    return;
  }

  m_renderer = SDL_CreateRenderer(
      m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!m_renderer) {
    std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
    return;
  }

  m_isRunning = true;
}

void Game::run() {
  while (m_isRunning) {
    processInput();
    update();
    render();
  }
}

void Game::processInput() {
  SDL_Event sdlEvent;

  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_QUIT:
      m_isRunning = false;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
        m_isRunning = false;
      }
      break;
    }
  }
}

void Game::update() {
  // TODO: update game objects
}

void Game::render() {
  SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
  SDL_RenderClear(m_renderer);

  // TODO: render all game objects

  SDL_RenderPresent(m_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}