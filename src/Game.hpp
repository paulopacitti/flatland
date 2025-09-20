#ifndef GAME_HPP
#define GAME_HPP

#include "AssetStore.hpp"
#include "ECS.hpp"
#include <SDL2/SDL.h>
#include <cstdint>
#include <memory>

const int FPS = 60;
const int MS_PER_FRAME = 1000 / FPS;

class Game {
  private:
    bool m_isRunning;
    uint64_t m_previousFrameTime;
    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<AssetStore> m_assetStore;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    void loadLevel(uint8_t level);
    void setup();
    void loadTilemap(std::string mapFilePath, std::string mapSpriteFilePath,
                     uint32_t tileSize, float scale);

    /**
     * Calculates the time elapsed since the last frame and enforces a
     * frame cap.
     *
     * This function calculates the delta time (in seconds) since the last
     * frame, enforces a frame cap by delaying the execution if necessary,
     * and updates the previous frame time.
     *
     * Returns the delta time (in seconds) since the last frame.
     */
    double getDeltaTime();

  public:
    uint16_t windowWidth;
    uint16_t windowHeight;

    Game();
    ~Game();
    void initialize();

    /**
     * Runs the main game loop.
     *
     * This function sets up the game and enters the main game loop, which
     * continues to run while the game is in a running state. Within the loop,
     * it processes input, updates the game state, and renders the game.
     */
    void run();

    /**
     * Processes input events for the game.
     *
     * This function polls for SDL events and handles them accordingly. It
     * checks for quit events and keydown events. If the quit event is
     * detected or the escape key is pressed, the game will stop running.
     */
    void processInput();
    void update();
    void render();
    void destroy();
};

#endif
