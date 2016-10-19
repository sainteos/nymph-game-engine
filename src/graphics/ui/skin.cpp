#include "skin.h"

namespace Graphics {
  namespace UI {

    Skin::Skin(std::shared_ptr<BaseTexture> texture, std::shared_ptr<Shader> shader) : texture(texture), shader(shader) {

    }

    void Skin::setTexture(const std::shared_ptr<BaseTexture>& texture) {
      this->texture = texture;
    }

    void Skin::setShader(const std::shared_ptr<Shader>& shader) {
      this->shader = shader;
    }

    std::shared_ptr<BaseTexture> Skin::getTexture() {
      return texture.lock();
    }

    std::shared_ptr<Shader> Skin::getShader() {
      return shader.lock();
    }
  }
}