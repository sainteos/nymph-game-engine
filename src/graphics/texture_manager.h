#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include "graphics/base_texture.h"

namespace Graphics {
  class TextureManager {
    private:
      std::map<std::string, std::shared_ptr<BaseTexture>> textures_to_names;
    public:
      TextureManager();
      ~TextureManager();

      const bool loadTexture(const std::string& path, const std::string& texture_uniform_name);

      const std::shared_ptr<BaseTexture> operator[](const std::string& name) const;
      const std::shared_ptr<BaseTexture> getTexture(const std::string& name) const;

      const bool textureExists(const std::string& name) const noexcept;

      static const std::string getNameFromPath(const std::string& path) noexcept;
  };  
}

#endif
