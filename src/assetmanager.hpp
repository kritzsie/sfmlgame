#pragma once

#include "types.hpp"

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace ke {
class AssetManager {
protected:
  bool load(std::string, std::string);
  virtual bool onLoad(sf::InputStream&, std::string, std::string) = 0;

  AssetManager(const StringList&);

public:
  const StringList extensions;

  AssetManager(const AssetManager&) = delete;
  virtual ~AssetManager() = default;
};

class GFXAssets : public AssetManager {
private:
  StringTable<StringTable<sf::Texture>> assets;

  bool onLoad(sf::InputStream&, std::string, std::string) final;

  GFXAssets();

public:
  static sf::Texture none;

  static GFXAssets& getInstance();

  bool loadSprite(std::string);
  bool loadTile(std::string);
  bool loadTexture(std::string);

  sf::Texture& getSprite(std::string);
  sf::Texture& getTexture(std::string);
  sf::Texture& getTile(std::string);

  GFXAssets(const GFXAssets&) = delete;
};

class SFXAssets : public AssetManager {
private:
  StringTable<sf::SoundBuffer> sounds;

  bool onLoad(sf::InputStream&, std::string, std::string) final;

  SFXAssets();

public:
  static sf::SoundBuffer none;

  static SFXAssets& getInstance();

  bool loadSound(std::string);

  sf::SoundBuffer& getSound(std::string);

  SFXAssets(const SFXAssets&) = delete;
};

namespace assets {
  extern GFXAssets& gfx;
  extern SFXAssets& sfx;
}
}
