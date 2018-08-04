#include "assetmanager.hpp"

#include <physfs.hpp>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include <stdexcept>
#include <string>

namespace ke {
template<>
inline void AssetManager<sf::Texture>::onLoad(sf::Texture& texture) {
  texture.setRepeated(true);
}

template<>
inline void AssetManager<sf::SoundBuffer>::onLoad(sf::SoundBuffer& texture) {}

template<typename T>
bool AssetManager<T>::load(std::string key) {
  const std::string folders[] = {"tiles", "sprites", "graphics"};
  const std::string extensions[] = {"png", "bmp", "jpg"};
  for (auto& dir : folders)
  for (auto& ext : extensions) {
    std::string filename = "/" + dir + "/" + key + "." + ext;
    if (PhysFS::exists(filename)) {
      T asset;
      PhysFS::ifstream fin(filename);
      std::size_t size = fin.length();
      char* data = new char[size];
      fin.read(data, size);
      asset.loadFromMemory(data, size);
      delete[] data;
      onLoad(asset);
      assets.emplace(key, asset);
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
}
