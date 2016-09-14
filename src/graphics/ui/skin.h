#ifndef SKIN_H
#define SKIN_H
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include "graphics/base_texture.h"
#include "graphics/shader.h"

namespace Graphics {
  namespace UI {
    struct Skin {
      std::weak_ptr<BaseTexture> texture;
      std::weak_ptr<Shader> shader;
    };
  }
}

#endif
