#include "assetmanager.hpp"

#include "types.hpp"
#include "util.hpp"

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <utility>

namespace ke {
static const StringList gfx_extensions {
  "hdr", "png", "psd", "tga", "bmp", "gif", "pic", "jpg", "jpeg", ""
};

static const StringList sfx_extensions {
  "flac", "wav", "ogg", ""
};

sf::Texture     GFXAssets::none;
sf::SoundBuffer SFXAssets::none;

GFXAssets& assets::gfx = GFXAssets::getInstance();
SFXAssets& assets::sfx = SFXAssets::getInstance();

bool AssetManager::load(std::string dir, std::string name) {
  for (auto& ext : extensions) {
    std::string path = "/" + dir + "/" + (ext.empty() ? name : name + "." + ext);
    if (PHYSFS_exists(path.c_str())) {
      util::FileInputStream ifs;

      ifs.open(path);

      return onLoad(ifs, dir, name);
    }
  }

  return false;
}

AssetManager::AssetManager(const StringList& exts) : extensions(exts) {}

bool GFXAssets::onLoad(util::FileInputStream& ifs, std::string dir, std::string name) {
  sf::Texture texture;

  texture.loadFromStream(ifs);
  texture.setRepeated(true);
  assets[dir].emplace(name, std::move(texture));

  return true;
}

bool GFXAssets::loadSprite(std::string name) {
  return load("sprites", name);
}

bool GFXAssets::loadTexture(std::string name) {
  return load("textures", name);
}

bool GFXAssets::loadTile(std::string name) {
  return load("tiles", name);
}

sf::Texture& GFXAssets::getTexture(std::string name) {
  try {
    return assets["textures"].at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTexture(name)) {
      return assets["textures"][name];
    }
  }

  return none;
}

sf::Texture& GFXAssets::getTile(std::string name) {
  try {
    return assets["tiles"].at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTile(name)) {
      return assets["tiles"][name];
    }
  }

  return getTexture(name);
}

sf::Texture& GFXAssets::getSprite(std::string name) {
  try {
    return assets["sprites"].at(name);
  }
  catch (const std::out_of_range&) {
    if (loadSprite(name)) {
      return assets["sprites"][name];
    }
  }

  return getTile(name);
}

GFXAssets& GFXAssets::getInstance() {
  static GFXAssets instance;

  return instance;
}

GFXAssets::GFXAssets() : AssetManager(gfx_extensions) {
  assets["sprites" ][""] = none;
  assets["tiles"   ][""] = none;
  assets["textures"][""] = none;
}

bool SFXAssets::onLoad(util::FileInputStream& ifs, std::string, std::string name) {
  sf::SoundBuffer sound;

  sound.loadFromStream(ifs);
  sounds.emplace(name, std::move(sound));

  return true;
}

bool SFXAssets::loadSound(std::string name) {
  return load("sounds", name);
}

sf::SoundBuffer& SFXAssets::getSound(std::string name) {
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

SFXAssets& SFXAssets::getInstance() {
  static SFXAssets instance;

  return instance;
}

SFXAssets::SFXAssets() : AssetManager(sfx_extensions) {
  sounds[""] = none;
}
}
