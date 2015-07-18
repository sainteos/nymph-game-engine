#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "component.h"

namespace Graphics {
  class Renderable : Component {
    private:
      bool active;

      static constexpr double quad_vertex_coords[12] = {0.0, 0.0, 0.0,
                                                    0.0, 1.0, 0.0,
                                                    1.0, 1.0, 0.0,
                                                    1.0, 0.0, 0.0};
      static constexpr double quad_texture_coords[12] = {0.0, 0.0, 0.0,
                                                     0.0, 1.0, 0.0,
                                                     1.0, 1.0, 0.0,
                                                     1.0, 0.0, 0.0};
      static constexpr int quad_indices[6] = {0, 1, 2, 0, 2, 3};

      unsigned int vertex_array_object;
    public:
      Renderable() = delete;
      Renderable(const unsigned int vertex_array_object);

      void setActive() noexcept;
      void setInactive() noexcept;
      const bool isActive() const noexcept;

      const unsigned int getVertexArrayBinding() const noexcept;
      
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
