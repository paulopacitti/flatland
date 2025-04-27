#include "AssetStore.hpp"
#include "SDL2/SDL_render.h"
#include "spdlog/spdlog.h"
#include <SDL2/SDL_image.h>

AssetStore::AssetStore() { spdlog::info("AssetStore initialized."); }

AssetStore::~AssetStore() {
  this->clearAssets();
  spdlog::info("[AssetStore] AssetStore destructed.");
}

void AssetStore::clearAssets() {
  for (auto texture : m_textures) {
    SDL_DestroyTexture(texture.second);
  }

  m_textures.clear();
}

void AssetStore::addTexture(SDL_Renderer* renderer, const std::string& assetId,
                            const std::string& filePath) {
  SDL_Surface* surface = IMG_Load(filePath.c_str());
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  m_textures.emplace(assetId, texture);
  spdlog::info("[AssetStore] New texture added to the AssetStore with id={}",
               assetId);
}

SDL_Texture* AssetStore::getTexture(const std::string& assetId) const {
  return m_textures.at(assetId);
}