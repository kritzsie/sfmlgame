#include "assetmanager.hpp"

#include "util.hpp"

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>

namespace ke {
const std::string GFXAssetManager::extensions[] = {
  "hdr", "png", "psd", "tga", "gif", "bmp", "pic", "jpg", "jpeg", ""
};

const std::string SFXAssetManager::extensions[] = {
  "flac", "wav", "ogg", ""
};

sf::Texture     GFXAssetManager::none;
sf::SoundBuffer SFXAssetManager::none;

bool GFXAssetManager::load(
  std::string dir,
  std::string name,
  std::map<std::string, sf::Texture>& container
) {
  for (auto& ext : extensions) {
    std::string path = dir + "/" + (ext.size() ? name + "." + ext : name);
    if (PHYSFS_exists(path.c_str())) {
      sf::Texture texture;
      util::FileInputStream filestream;

      filestream.open(path);
      texture.loadFromStream(filestream);
      texture.setRepeated(true);
      container.emplace(name, texture);

      return true;
    }
  }

  return false;
}

bool GFXAssetManager::load(std::string dir, std::string name) {
  return load(dir, name, textures);
}

bool GFXAssetManager::loadSprite(std::string name) {
  return load("/sprites", name, sprites);
}

bool GFXAssetManager::loadTexture(std::string name) {
  return load("/textures", name);
}

bool GFXAssetManager::loadTile(std::string name) {
  return load("/tiles", name, tiles);
}

sf::Texture& GFXAssetManager::getSprite(std::string name) {
  if (name == "") {
    return none;
  }

  try {
    return sprites.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadSprite(name)) {
      return sprites[name];
    }
  }

  try {
    return tiles.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTile(name)) {
      return tiles[name];
    }
  }

  try {
    return textures.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTexture(name)) {
      return textures[name];
    }
  }

  return none;
}

sf::Texture& GFXAssetManager::getTexture(std::string name) {
  if (name == "") {
    return none;
  }

  try {
    return textures.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTexture(name)) {
      return textures[name];
    }
  }

  return none;
}

sf::Texture& GFXAssetManager::getTile(std::string name) {
  if (name == "") {
    return none;
  }

  try {
    return tiles.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTile(name)) {
      return tiles[name];
    }
  }

  try {
    return textures.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTexture(name)) {
      return textures[name];
    }
  }

  return none;
}

bool SFXAssetManager::load(std::string dir, std::string name) {
  for (auto& ext : extensions) {
    std::string path = dir + "/" + (ext.size() ? name + "." + ext : name);
    if (PHYSFS_exists(path.c_str())) {
      util::FileInputStream filestream;
      sf::SoundBuffer sound;

      filestream.open(path);
      sound.loadFromStream(filestream);
      sounds.emplace(name, sound);

      return true;
    }
  }

  return false;
}

bool SFXAssetManager::loadSound(std::string name) {
  return load("/sounds", name);
}

sf::SoundBuffer& SFXAssetManager::getSound(std::string name) {
  if (name == "") {
    return none;
  }

  try {
    return sounds.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadSound(name)) {
      return sounds[name];
    }
  }

  return none;
}
}
