#ifndef RENDERABLE_ATTRIBUTE_TRAIT_H
#define RENDERABLE_ATTRIBUTE_TRAIT_H
#include <map>
#include "graphics/vertex_data.h"

namespace Graphics {
  class RenderableAttributeTrait {
    public:
      const virtual std::map<VertexData::DATA_TYPE, unsigned int> operator()() const noexcept = 0;
  };
}
#endif
