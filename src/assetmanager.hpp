#pragma once

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace ke {
std::vector<char> readfile(std::string);

class GFXAssetManager {
public:
  static const std::string extensions[];
  static sf::Texture none;

private:
  std::map<std::string, sf::Texture> sprites, textures, tiles;

  bool load(std::string, std::string);
  bool load(std::string, std::string, std::map<std::string, sf::Texture>&);

public:
  bool loadSprite(std::string);
  bool loadTexture(std::string);
  bool loadTile(std::string);

  sf::Texture& getSprite(std::string);
  sf::Texture& getTexture(std::string);
  sf::Texture& getTile(std::string);
};

class SFXAssetManager {
public:
  static const std::string extensions[];
  static sf::SoundBuffer none;

private:
  std::map<std::string, sf::SoundBuffer> sounds;

  bool load(std::string, std::string);

public:
  bool loadSound(std::string);

  sf::SoundBuffer& getSound(std::string);
};
}
