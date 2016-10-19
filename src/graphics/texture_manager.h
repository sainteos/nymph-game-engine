#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include "graphics/base_texture.h"
//= SCRIPTABLE

namespace Graphics {
  class TextureManager {
    private:
      std::map<std::string, std::shared_ptr<BaseTexture>> textures_to_names;
    public:
      TextureManager();
      ~TextureManager();

      //= BEGIN SCRIPTABLE
      const bool loadTexture(const std::string& path);

      std::shared_ptr<BaseTexture> operator[](const std::string& name) const;
      std::shared_ptr<BaseTexture> getTexture(const std::string& name) const;

      const bool textureExists(const std::string& name) const noexcept;
      //= END SCRIPTABLE

      static const std::string getNameFromPath(const std::string& path) noexcept;
  };  
}

#endif
