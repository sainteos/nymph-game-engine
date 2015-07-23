#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <glm/vec3.hpp>
#include <map>
#include "component.h"
#include "vertex_data.h"
#include "renderable_attribute_trait.h"
#include "tile_attribute_trait.h"

namespace Graphics {
  class Renderable : Component {
    private:
      bool active;
      bool initialized;

      unsigned int vertex_array_object;
      unsigned int num_of_vertex_buffers;
      unsigned int* vertex_buffer_objects;
      unsigned int index_buffer_object;
      unsigned int shader_object;

      VertexData vertex_data;
      RenderableAttributeTrait* trait;
    public:

      Renderable() = delete;
      Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data, RenderableAttributeTrait* trait = new TileAttributeTrait());
      ~Renderable();
      
      void initialize();
      const bool isInitialized() const noexcept;

      void setActive() noexcept;
      void setInactive() noexcept;
      const bool isActive() const noexcept;

      void setShader(const unsigned int shader_object);
      const unsigned int getShaderBinding() const noexcept;

      const unsigned int getVertexArrayBinding() const noexcept;
      const unsigned int getVertexBufferBinding(const VertexData::DATA_TYPE& data_type);
      const unsigned int getIndexBufferBinding() const noexcept;
      
      void destroy();
      /**
       * @brief [brief description]
       * @details [long description]
       * @return true if anything was updated, false if nothing was updated
       */
      virtual const bool onUpdate(const double delta) override;
      virtual const bool onRender();
  };
}
#endif
