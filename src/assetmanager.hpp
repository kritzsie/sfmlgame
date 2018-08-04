#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace ke {
template<typename T>
class AssetManager {
private:
  std::map<std::string, T> assets;

protected:
  void onLoad(T&);

public:
  bool load(std::string);
  T& operator [](std::string);
};

using GFXAssetManager = AssetManager<sf::Texture>;
using SFXAssetManager = AssetManager<sf::SoundBuffer>;
}

#include "assetmanager.cpp"
