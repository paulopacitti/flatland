#include "Game.hpp"
#include "ECS.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "spdlog/spdlog.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
  m_isRunning = false;
  m_previousFrameTime = 0;
  spdlog::info("Game constructor called");
}

Game::~Game() { spdlog::info("Game destructor called"); }

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

void Game::setup() {
  // TODO:
  // Entity tank = regitry.createEntity();
  // tank.addComponent<TransformCompoenent>();
  // tank.addComponent<BoxColliderComponent>();
  // tank.addComponent<SpriteComponent>("./assets/images/tank.png");
}

/**
 * Runs the main game loop.
 *
 * This function sets up the game and enters the main game loop, which continues
 * to run while the game is in a running state. Within the loop, it processes
 * input, updates the game state, and renders the game.
 */
void Game::run() {
  setup();
  while (m_isRunning) {
    processInput();
    update();
    render();
  }
}

/**
 * Processes input events for the game.
 *
 * This function polls for SDL events and handles them accordingly. It checks
 * for quit events and keydown events. If the quit event is detected or the
 * escape key is pressed, the game will stop running.
 */
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

/**
 * Calculates the time elapsed since the last frame and enforces a
 * frame cap.
 *
 * This function calculates the delta time (in seconds) since the last frame,
 * enforces a frame cap by delaying the execution if necessary, and updates
 * the previous frame time.
 *
 * Returns the delta time (in seconds) since the last frame.
 */
double Game::getDeltaTime() {
  // frame cap
  uint64_t waitMs = MS_PER_FRAME - (SDL_GetTicks64() - m_previousFrameTime);
  if (waitMs > 0 && waitMs <= MS_PER_FRAME) {
    // sleep until next frame
    SDL_Delay(static_cast<uint32_t>(waitMs));
  }

  // delta time (in seconds)
  double dt = (SDL_GetTicks64() - m_previousFrameTime) / 1000.0;
  // store the current frame time
  m_previousFrameTime = SDL_GetTicks64();

  return dt;
}

void Game::update() {
  double dt = getDeltaTime();

  // TODO:
  // MovementSystem.update();
  // CollitionSystem.update();
  // UpdateSystem.update();
}

void Game::render() {
  // draw background
  SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
  SDL_RenderClear(m_renderer);

  // TODO: Render game objects.

  // render buffer
  SDL_RenderPresent(m_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}