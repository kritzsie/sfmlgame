#include "assetmanager.hpp"

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <sstream>
#include <string>

namespace ke {
const std::string GFXAssetManager::extensions[] = {
  "hdr", "png", "psd", "tga", "gif", "bmp", "pic", "jpg", "jpeg"
};

const std::string SFXAssetManager::extensions[] = {
  "flac", "wav", "ogg"
};

sf::Texture     GFXAssetManager::none = sf::Texture();
sf::SoundBuffer SFXAssetManager::none = sf::SoundBuffer();

std::vector<char> readfile(std::string path) {
  PHYSFS_File* file = PHYSFS_openRead(path.c_str());
  std::size_t size = PHYSFS_fileLength(file);
  char* data = new char[size];
  if (file == nullptr) {
    std::stringstream ss;
    ss << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
    throw std::runtime_error(ss.str());
  }
  PHYSFS_readBytes(file, data, size);
  PHYSFS_close(file);
  return std::vector<char>(data, data + size);
}

bool GFXAssetManager::load(std::string dir, std::string name, std::map<std::string, sf::Texture>& container) {
  for (auto& ext : extensions) {
    std::string path = dir + "/" + name + "." + ext;
    if (PHYSFS_exists(path.c_str())) {
      std::vector<char> file = readfile(path);
      sf::Texture texture;
      texture.loadFromMemory(file.data(), file.size());
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
    std::string path = dir + "/" + name + "." + ext;
    if (PHYSFS_exists(path.c_str())) {
      std::vector<char> file = readfile(path);
      sf::SoundBuffer sound;
      sound.loadFromMemory(file.data(), file.size());
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
