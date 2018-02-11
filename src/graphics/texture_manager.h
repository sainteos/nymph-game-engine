#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include "base_texture.h"

namespace Graphics {
  /**
   * @brief      Class for texture manager.
   */
  class [[scriptable]] TextureManager {
    private:
      std::map<std::string, std::shared_ptr<BaseTexture>> textures_to_names;
    public:
      /**
       * @brief      TextureManager constructor
       */
      TextureManager();
      /**
       * @brief      Destroys the object.
       */
      ~TextureManager();

      /**
       * @brief      Loads a texture.
       *
       * @param[in]  path  The path
       *
       * @return     True if successful
       */
      [[scriptable]] bool loadTexture(const std::string& path);

      /**
       * @brief      Get's texture with name
       *
       * @param[in]  name  The name
       *
       * @return     A texture
       */
      std::shared_ptr<BaseTexture> operator[](const std::string& name) const;
      /**
       * @brief      Gets the texture.
       *
       * @param[in]  name  The name
       *
       * @return     The texture.
       */
      [[scriptable]] std::shared_ptr<BaseTexture> getTexture(const std::string& name) const;

      /**
       * @brief      Function to see if texture with name exists
       *
       * @param[in]  name  The name
       *
       * @return     True if it exists
       */
      [[scriptable]] bool textureExists(const std::string& name) const noexcept;

      /**
       * @brief      Gets the name from path.
       *
       * @param[in]  path  The path
       *
       * @return     The name from path.
       */
      static std::string getNameFromPath(const std::string& path) noexcept;
  };
}

#endif
