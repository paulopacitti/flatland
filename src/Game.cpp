#include "Game.hpp"
#include "Component.hpp"
#include "ECS.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "glm/ext/vector_float2.hpp"
#include "spdlog/spdlog.h"
#include "systems/MovementSystem.hpp"
#include "systems/RenderSystem.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdint>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <string>

Game::Game() {
  m_isRunning = false;
  m_previousFrameTime = 0;
  m_registry = std::make_unique<Registry>();
  m_assetStore = std::make_unique<AssetStore>();
  spdlog::info("[Game] created.");
}

Game::~Game() { spdlog::info("[Game] destroyed."); }

void Game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    spdlog::error("Error initializing SDL: {}", SDL_GetError());
    return;
  }

  windowWidth = 800;
  windowHeight = 600;
  m_window = SDL_CreateWindow("flatland", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                              SDL_WINDOW_RESIZABLE);
  if (!m_window) {
    spdlog::error("Error creating SDL window: {}", SDL_GetError());
    return;
  }

  m_renderer = SDL_CreateRenderer(
      m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!m_renderer) {
    spdlog::error("Error creating SDL renderer: {}", SDL_GetError());
    return;
  }

  m_isRunning = true;
}

void Game::loadLevel(uint8_t level) {
  m_registry->addSystem<MovementSystem>();
  m_registry->addSystem<RenderSystem>();

  // adding assets to the AssetStore
  m_assetStore->addTexture(m_renderer, "tank-image",
                           "../assets/images/tank-panther-right.png");
  m_assetStore->addTexture(m_renderer, "truck-image",
                           "../assets/images/truck-ford-right.png");

  loadTilemap("./assets/tilemaps/jungle.map", "../assets/tilemaps/jungle.png",
              32, 1.5);

  Entity tank = m_registry->createEntity();
  tank.addComponent<TransformComponent>(glm::vec2(10.0, 30.0),
                                        glm::vec2(1.0, 1.0), 45.0);
  tank.addComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
  tank.addComponent<SpriteComponent>("tank-image", 32, 32);

  Entity truck = m_registry->createEntity();
  truck.addComponent<TransformComponent>(glm::vec2(50.0, 100.0),
                                         glm::vec2(1.0, 1.0), 0.0);
  truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0));
  truck.addComponent<SpriteComponent>("truck-image", 32, 32);
}

void Game::setup() { loadLevel(1); }

void Game::run() {
  setup();
  while (m_isRunning) {
    processInput();
    update();
    render();
  }
}

void Game::loadTilemap(std::string mapFilePath, std::string spriteFilePath,
                       uint32_t tileSize, float scale) {
  std::fstream mapFile;
  std::filesystem::path filePath(mapFilePath);
  if (!std::filesystem::exists(filePath)) {
    spdlog::error("Map file does not exist: {}", mapFilePath);
    return;
  }

  m_assetStore->addTexture(m_renderer, "tilemap-image", spriteFilePath);
  mapFile.open(mapFilePath);
  if (!mapFile.is_open()) {
    spdlog::error("Failed to open map file: {}", mapFilePath);
    return;
  }

  std::string line;
  int i = 0;
  while (std::getline(mapFile, line, '\n')) {
    int j = 0;
    std::stringstream ss(line);
    std::string tileIndexStr;

    while (std::getline(ss, tileIndexStr, ',')) {
      int tileIndex = std::stoi(tileIndexStr);

      // Extract the two digits from the tileIndex
      int srcRectY = (tileIndex / 10) * tileSize;
      int srcRectX = (tileIndex % 10) * tileSize;

      glm::vec2 vecPosition(j * tileSize * scale, i * tileSize * scale);
      glm::vec2 vecScale(scale, scale);

      Entity tile = m_registry->createEntity();
      tile.addComponent<TransformComponent>(vecPosition, vecScale, 0);
      tile.addComponent<SpriteComponent>("tilemap-image", tileSize, tileSize,
                                         srcRectX, srcRectY);

      j++;
    }
    i++;
  }

  mapFile.close();
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
  m_registry->update();

  m_registry->getSystem<MovementSystem>().update(dt);
}

void Game::render() {
  // draw background
  SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
  SDL_RenderClear(m_renderer);

  // draw backgrounds
  m_registry->getSystem<RenderSystem>().update(m_renderer, *m_assetStore);
  // render buffer
  SDL_RenderPresent(m_renderer);
}

void Game::destroy() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}
