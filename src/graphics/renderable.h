#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <glm/vec3.hpp>
#include "component.h"

namespace Graphics {
  class Renderable : Component {
    private:
      bool active;
      bool initialized;

      unsigned int vertex_array_object;
      unsigned int num_of_vertex_buffers;
      unsigned int* vertex_buffer_objects;
      unsigned int index_buffer_object;
    public:

      Renderable() = delete;
      Renderable(const unsigned int vertex_array_object);
      ~Renderable();
      
      void initialize(const std::vector<std::vector<glm::vec3>>& vertex_data, const std::vector<unsigned int>& index_data = std::vector<unsigned int>());
      const bool isInitialized() const noexcept;

      void setActive() noexcept;
      void setInactive() noexcept;
      const bool isActive() const noexcept;

      const unsigned int getVertexArrayBinding() const noexcept;
      const unsigned int getVertexBufferBinding(const unsigned int index) const;
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
