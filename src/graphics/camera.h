#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <memory>
#include "component.h"
#include "transform.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"

namespace Graphics {
  class Camera : Component {
    private:
      std::shared_ptr<Transform> transform;
      glm::mat4 projection_matrix;
      //THESE NEED TO BE MOVED TO SOMEWHERE ELSE, PERHAPS AN INHERITED CLASS
      Transform last_transform;
      glm::mat4 last_projection_matrix;

      std::shared_ptr<ShaderManager> shader_manager;
      float viewport_width;
      float viewport_height;
      float near;
      float far;

      Transform negateTransformForScreen(std::shared_ptr<Transform> trans);
    public:
      Camera() = delete;
      Camera(const std::shared_ptr<ShaderManager> shader_manager);
      Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near = 0.1, const float far = 1.0);
      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual void destroy() override;

      void setWidth(const float width) noexcept;
      const float getWidth() const noexcept;
      void setHeight(const float height) noexcept;
      const float getHeight() const noexcept;
      void setNear(const float near) noexcept;
      const float getNear() const noexcept;
      void setFar(const float far) noexcept;
      const float getFar() const noexcept;

      const bool isRenderableWithin(std::shared_ptr<Renderable> renderable) const;

      void setTransform(const std::shared_ptr<Transform> transform) noexcept;
      std::shared_ptr<Transform> getTransform() const noexcept;
  };
}
#endif
