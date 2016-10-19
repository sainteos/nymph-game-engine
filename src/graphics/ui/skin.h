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
    class Skin {
      private:
        std::weak_ptr<BaseTexture> texture;
        std::weak_ptr<Shader> shader;
      public:
      //= BEGIN SCRIPTABLE
        Skin(std::shared_ptr<BaseTexture> texture, std::shared_ptr<Shader> shader);
        
        void setTexture(const std::shared_ptr<BaseTexture>& texture);
        void setShader(const std::shared_ptr<Shader>& shader);
        std::shared_ptr<BaseTexture> getTexture();
        std::shared_ptr<Shader> getShader();
      //= END SCRIPTABLE
    };
  }
}

#endif
