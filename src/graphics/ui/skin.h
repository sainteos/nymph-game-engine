#ifndef SKIN_H
#define SKIN_H
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include "graphics/base_texture.h"
#include "graphics/shader.h"
//= SCRIPTABLE

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI skin.
     */
    class Skin {
      private:
        std::weak_ptr<BaseTexture> texture;
        std::weak_ptr<Shader> shader;
      public:
      //= BEGIN SCRIPTABLE
      
        /**
         * @brief      Skin constructor
         *
         * @param[in]  texture  The texture
         * @param[in]  shader   The shader
         */
        Skin(std::shared_ptr<BaseTexture> texture, std::shared_ptr<Shader> shader);
        
        /**
         * @brief      Sets the texture.
         *
         * @param[in]  texture  The texture
         */
        void setTexture(const std::shared_ptr<BaseTexture>& texture);
        /**
         * @brief      Sets the shader.
         *
         * @param[in]  shader  The shader
         */
        void setShader(const std::shared_ptr<Shader>& shader);
        /**
         * @brief      Gets the texture.
         *
         * @return     The texture.
         */
        std::shared_ptr<BaseTexture> getTexture();
        /**
         * @brief      Gets the shader.
         *
         * @return     The shader.
         */
        std::shared_ptr<Shader> getShader();
      //= END SCRIPTABLE
    };
  }
}

#endif
