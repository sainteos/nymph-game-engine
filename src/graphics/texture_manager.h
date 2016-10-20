#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include "graphics/base_texture.h"
//= SCRIPTABLE

namespace Graphics {
  /**
   * @brief      Class for texture manager.
   */
  class TextureManager {
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

      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Loads a texture.
       *
       * @param[in]  path  The path
       *
       * @return     True if successful
       */
      const bool loadTexture(const std::string& path);

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
      std::shared_ptr<BaseTexture> getTexture(const std::string& name) const;

      /**
       * @brief      Function to see if texture with name exists
       *
       * @param[in]  name  The name
       *
       * @return     True if it exists
       */
      const bool textureExists(const std::string& name) const noexcept;
      //= END SCRIPTABLE

      /**
       * @brief      Gets the name from path.
       *
       * @param[in]  path  The path
       *
       * @return     The name from path.
       */
      static const std::string getNameFromPath(const std::string& path) noexcept;
  };  
}

#endif
