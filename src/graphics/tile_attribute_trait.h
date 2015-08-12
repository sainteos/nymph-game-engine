#ifndef TILE_ATTRIBUTE_TRAIT
#define TILE_ATTRIBUTE_TRAIT
#include <map>
#include "graphics/base_attribute_trait.h"
#include "graphics/vertex_data.h"

namespace Graphics {
  class TileAttributeTrait : public BaseAttributeTrait {
    private:
      std::map<VertexData::DATA_TYPE, unsigned int> trait;
    public:
      TileAttributeTrait() {
        trait = std::map<VertexData::DATA_TYPE, unsigned int> {
          {VertexData::DATA_TYPE::GEOMETRY, 0},
          {VertexData::DATA_TYPE::TEX_COORDS, 1}
        };
      }

      virtual const std::map<VertexData::DATA_TYPE, unsigned int> operator()() const noexcept override {
        return trait;
      }
  };
}
#endif
