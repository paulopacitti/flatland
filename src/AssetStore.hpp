#ifndef ASSETSTORE_HPP
#define ASSETSTORE_HPP
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class AssetStore {
  private:
    std::unordered_map<std::string, SDL_Texture*> m_textures;
    // TODO: create collection of fonts
    // TODO: create collection of audio

  public:
    AssetStore();
    ~AssetStore();

    void clearAssets();
    void addTexture(SDL_Renderer* renderer, const std::string& assetId,
                    const std::string& filePath);
    SDL_Texture* getTexture(const std::string& assetId) const;
};

#endif