#include "assetmanager.hpp"

#include <physfs.hpp>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include <stdexcept>
#include <string>

namespace ke {
template<>
inline bool AssetManager<sf::Texture>::load(std::string key) {
  std::list<std::string> folders = {"tiles", "sprites", "graphics"};
  std::list<std::string> extensions = {"png", "bmp", "jpg"};
  for (auto& dir : folders)
  for (auto& ext : extensions) {
    std::string filename = "/" + dir + "/" + key + "." + ext;
    if (PhysFS::exists(filename)) {
      sf::Texture texture;
      PhysFS::ifstream fin(filename);
      std::size_t size = fin.length();
      char* data = new char[size];
      fin.read(data, size);
      texture.loadFromMemory(data, size);
      delete[] data;
      texture.setRepeated(true);
      assets.emplace(key, texture);
      return true;
    }
  }
  return false;
}

template<>
inline bool AssetManager<sf::SoundBuffer>::load(std::string key) {
  std::list<std::string> folders = {"tiles", "sprites", "graphics"};
  std::list<std::string> extensions = {"png", "bmp", "jpg"};
  for (auto& dir : folders)
  for (auto& ext : extensions) {
    std::string filename = "/" + dir + "/" + key + "." + ext;
    if (PhysFS::exists(filename)) {
      sf::SoundBuffer sound;
      PhysFS::ifstream fin(filename);
      std::size_t size = fin.length();
      char* data = new char[size];
      fin.read(data, size);
      sound.loadFromMemory(data, size);
      delete[] data;
      assets.emplace(key, sound);
      return true;
    }
  }
  return false;
}

template<typename T>
T& AssetManager<T>::operator [](std::string key) {
  try {
    return assets.at(key);
  }
  catch (std::out_of_range& ex) {
    if (load(key)) {
      return assets.at(key);
    }
    return assets.at("None");
  }
}

/*
bool GFXAssetManager::load(std::string key) {
  std::list<std::string> folders = {"tiles", "sprites", "graphics"};
  std::list<std::string> extensions = {"png", "bmp", "jpg"};
  for (auto& dir : folders)
  for (auto& ext : extensions) {
    std::string filename = "/" + dir + "/" + key + "." + ext;
    if (PhysFS::exists(filename)) {
      sf::Texture texture;
      PhysFS::ifstream fin(filename);
      std::size_t size = fin.length();
      char* data = new char[size];
      fin.read(data, size);
      texture.loadFromMemory(data, size);
      delete[] data;
      //texture.setRepeated(true);
      assets.emplace(key, texture);
      return true;
    }
  }
  return false;
}

sf::Texture& GFXAssetManager::operator [](std::string key) {
  try {
    return assets.at(key);
  }
  catch (std::out_of_range& e) {
    if (load(key)) {
      return assets.at(key);
    }
    return assets.at("None");
  }
}
*/
}
