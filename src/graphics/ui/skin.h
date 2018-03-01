#ifndef SKIN_H
#define SKIN_H
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include "../base_texture.h"
#include "../shader.h"
namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI skin.
     */
    class [[scriptable]] Skin {
      private:
        std::weak_ptr<BaseTexture> texture;
        std::weak_ptr<Shader> shader;
      public:
        /**
         * @brief      Skin constructor
         *
         * @param[in]  texture  The texture
         * @param[in]  shader   The shader
         */
        [[scriptable]] Skin(std::shared_ptr<BaseTexture> texture, std::shared_ptr<Shader> shader);

        /**
         * @brief      Sets the texture.
         *
         * @param[in]  texture  The texture
         */
        [[scriptable]] void setTexture(const std::shared_ptr<BaseTexture>& texture);
        /**
         * @brief      Sets the shader.
         *
         * @param[in]  shader  The shader
         */
        [[scriptable]] void setShader(const std::shared_ptr<Shader>& shader);
        /**
         * @brief      Gets the texture.
         *
         * @return     The texture.
         */
        [[scriptable]] std::shared_ptr<BaseTexture> getTexture();
        /**
         * @brief      Gets the shader.
         *
         * @return     The shader.
         */
        [[scriptable]] std::shared_ptr<Shader> getShader();
    };
  }
}

#endif
