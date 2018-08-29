#pragma once

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace ke {
class GFXAssetManager {
private:
  std::map<std::string, sf::Texture> sprites, textures, tiles;

  bool load(std::string, std::string, std::map<std::string, sf::Texture>&);

public:
  static const std::string extensions[];
  static sf::Texture none;

  static GFXAssetManager& getInstance() noexcept;

  bool load(std::string, std::string);
  bool loadSprite(std::string);
  bool loadTexture(std::string);
  bool loadTile(std::string);

  sf::Texture& getSprite(std::string);
  sf::Texture& getTexture(std::string);
  sf::Texture& getTile(std::string);

private:
  GFXAssetManager() = default;

public:
  GFXAssetManager(const GFXAssetManager&) = delete;
};

class SFXAssetManager {
private:
  std::map<std::string, sf::SoundBuffer> sounds;

public:
  static const std::string extensions[];
  static sf::SoundBuffer none;

  static SFXAssetManager& getInstance() noexcept;

  bool load(std::string, std::string);
  bool loadSound(std::string);

  sf::SoundBuffer& getSound(std::string);

private:
  SFXAssetManager() = default;

public:
  SFXAssetManager(const SFXAssetManager&) = delete;
};

namespace assets {
  extern GFXAssetManager& gfx;
  extern SFXAssetManager& sfx;
}
}
