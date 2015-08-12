#ifndef BASE_ATTRIBUTE_TRAIT
#define BASE_ATTRIBUTE_TRAIT

#include <map>
#include "graphics/vertex_data.h"

namespace Graphics {
  class BaseAttributeTrait {
    public:
      const virtual std::map<VertexData::DATA_TYPE, unsigned int> operator()() const noexcept = 0;
  };
}

#endif
