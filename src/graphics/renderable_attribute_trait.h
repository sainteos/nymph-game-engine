#ifndef RENDERABLE_ATTRIBUTE_TRAIT_H
#define RENDERABLE_ATTRIBUTE_TRAIT_H
#include <map>
#include "graphics/vertex_data.h"
#include "graphics/base_attribute_trait.h"

namespace Graphics {
  class RenderableAttributeTrait : public BaseAttributeTrait {
    private:
      std::map<VertexData::DATA_TYPE, unsigned int> trait;
    public:
      RenderableAttributeTrait() {
        trait = std::map<VertexData::DATA_TYPE, unsigned int> {
          {VertexData::DATA_TYPE::GEOMETRY, 0},
        };
      }

      virtual const std::map<VertexData::DATA_TYPE, unsigned int> operator()() const noexcept override {
        return trait;
      }
  };
}
#endif
