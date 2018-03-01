#include <easylogging++.h>
#include <fstream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif
#include "texture_manager.h"
#include "exceptions/invalid_texture_name_exception.h"

namespace Graphics {

  TextureManager::TextureManager() {

  }

  TextureManager::~TextureManager() {

  }

  bool TextureManager::loadTexture(const std::string& path) {
    std::string tex_name = getNameFromPath(path);
    auto texture = std::make_shared<BaseTexture>(GL_TEXTURE_2D);
    texture->setName(tex_name);
    if(texture->load(path)) {
      textures_to_names[tex_name] = texture;
      return true;
    }
    else {
      return false;
    }
  }

  std::shared_ptr<BaseTexture> TextureManager::operator[](const std::string& name) const {
    return getTexture(name);
  }

  std::shared_ptr<BaseTexture> TextureManager::getTexture(const std::string& name) const {
    if(textures_to_names.count(name) == 0) {
      throw Exceptions::InvalidTextureNameException(name);
    }
    return textures_to_names.at(name);
  }

  bool TextureManager::textureExists(const std::string& name) const noexcept {
    if(textures_to_names.count(name) > 0)
      return true;
    else
      return false;
  }

  std::string TextureManager::getNameFromPath(const std::string& path) noexcept {
    std:: string name;
    //plus one to remove the slash :)
    auto start = path.find_last_of("/") + 1;
    auto end = path.find_last_of(".");
    name = path.substr(start, end - start);
    return name;
  }
}
